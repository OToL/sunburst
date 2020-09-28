#include "local_file_system.h"
#include <sb_core/io/virtual_file_system.h>
#include <sb_core/io/path.h>
#include <sb_core/memory/allocator/pool_allocator.h>
#include <sb_core/memory/provider/global_heap_provider.h>
#include <sb_core/memory/alloc.h>
#include <sb_core/container/static_vector.h>
#include <sb_core/string/utility.h>

#include <sb_std/algorithm>
#include <sb_std/xutility>

namespace sb {

class VirtualFileSystemImpl
{
    struct LayerDesc
    {
        VFS::LayerName id;
        StaticString<VFS_PATH_MAX_LEN + 1> vfs_path;
        StaticString<LOCAL_PATH_MAX_LEN + 1> local_path;
    };

    using FileGen = ui16;

    struct FileDesc
    {
        internal::LayerFileHdl hdl;
        ui16 gen;
    };

    union FileHdlHelper
    {
        struct
        {
            ui16 hdl;
            ui16 gen;
        } unpacked;

        FileHdl::ValueType packed;
    };
    static_assert(sizeof(FileHdlHelper) == sizeof(FileHdl));

    VirtualFileSystemImpl(VirtualFileSystem const &) = delete;
    VirtualFileSystemImpl(VirtualFileSystem &&) = delete;

public:
    LayerDesc * findLayer(VFS::LayerName layer_id)
    {
        auto const layer_iter =
            sbstd::find_if(begin(layers), end(layers), [layer_id](LayerDesc const & layer_desc) {
                return layer_desc.id == layer_id;
            });

        return (layer_iter == end(layers)) ? nullptr : layer_iter;
    }

    VirtualFileSystemImpl(sbstd::span<VFS::LayerInitDesc> layers_desc)
        : file_desc_pool({VFS_FILE_MAX_OPENED})
        , curr_file_gen(0U)
        , opened_file_count(0U)
    {
        sbAssert(layers_desc.size() <= VFS_LAYER_MAX_COUNT);

        for (auto & layer_desc : layers_desc)
        {
            if (!(sbExpect(isValid(layer_desc.name), "VFS layer must have a valid name") &&
                  sbExpect(nullptr == findLayer(layer_desc.name),
                           "VFS layer already registered with the same name") &&
                  sbExpect(isVFSPathValid(layer_desc.vfs_path.c_str()), "Invalid layer VFS path")))
            {
                continue;
            }

            auto & new_layer =
                layers.emplace_back(layer_desc.name, layer_desc.vfs_path, layer_desc.local_path);

            if (VFS_PATH_SEPARATOR != new_layer.vfs_path.back())
            {
                if (!sbExpect(new_layer.vfs_path.push_back(VFS_PATH_SEPARATOR)))
                {
                    layers.pop_back();
                    continue;
                }
            }

            if (LOCAL_PATH_SEPARATOR != new_layer.local_path.back())
            {
                if (!sbExpect(new_layer.local_path.push_back(LOCAL_PATH_SEPARATOR)))
                {
                    layers.pop_back();
                    continue;
                }
            }
        }
    }

    ~VirtualFileSystemImpl()
    {
        sbWarn(0 == opened_file_count,
               "Not all files have been closed before destroying the File System");
    }

    FileDesc & getFileDesc(FileHdl hdl)
    {
        sbAssert(isValid(hdl));

        FileHdlHelper helper_hdl = {.packed = hdl.value};

        MemoryArena arena = file_desc_pool.getArena();
        FileDesc * const file_desc = static_cast<FileDesc *>(arena.m_ptr) + helper_hdl.unpacked.hdl;

        sbAssert(arena.isInRange(file_desc));
        sbAssert(helper_hdl.unpacked.gen == file_desc->gen);

        return *file_desc;
    }

    void closeFile(FileHdl hdl)
    {
        sbAssert(0 < opened_file_count);

        auto & desc = getFileDesc(hdl);
        internal::platformCloseFile(desc.hdl);

        destroyFileDesc(hdl);

        --opened_file_count;
    }

    FileSize readFile(FileHdl hdl, ui8 * buffer, FileSize cnt)
    {
        auto const & desc = getFileDesc(hdl);
        return internal::platformReadFile(desc.hdl, buffer, cnt);
    }

    FileSize getFileLength(FileHdl hdl)
    {
        auto const & desc = getFileDesc(hdl);
        return internal::platformFileLength(desc.hdl);
    }

    FileHdl openFileRead(char const * path, FileFormat fmt)
    {
        for (auto const & layer_desc : layers)
        {
            if (strStartWith(path, layer_desc.vfs_path.c_str()))
            {
                char local_file_path[LOCAL_PATH_MAX_LEN + 1];

                strCpyT(local_file_path, layer_desc.local_path.c_str());
                concatLocalPath(local_file_path, layer_desc.local_path.length(),
                                path + layer_desc.vfs_path.length());
                normalizeLocalPath(local_file_path);

                auto const local_file_hdl = internal::platformOpenFileRead(local_file_path, fmt);

                if (isValid(local_file_hdl))
                {
                    FileHdl file_hdl = createFileHdl(local_file_hdl);

                    if (sbExpect(isValid(file_hdl), "Out of file descriptors"))
                    {
                        ++opened_file_count;
                        return file_hdl;
                    }
                    else
                    {
                        internal::platformCloseFile({local_file_hdl});
                    }

                    break;
                }
            }
        }

        return {};
    }

    FileHdl openFileWrite(char const * path, FileWriteMode mode, FileFormat fmt)
    {
        for (auto const & layer_desc : layers)
        {
            if (strStartWith(path, layer_desc.vfs_path.c_str()))
            {
                char local_file_path[LOCAL_PATH_MAX_LEN + 1];

                strCpyT(local_file_path, layer_desc.local_path.c_str());
                concatLocalPath(local_file_path, layer_desc.local_path.length(),
                                path + layer_desc.vfs_path.length());
                normalizeLocalPath(local_file_path);

                auto const local_file_hdl =
                    internal::platformOpenFileWrite(local_file_path, mode, fmt);

                if (isValid(local_file_hdl))
                {
                    FileHdl file_hdl = createFileHdl(local_file_hdl);

                    if (sbExpect(isValid(file_hdl), "Out of file descriptors"))
                    {
                        ++opened_file_count;
                        return file_hdl;
                    }
                    else
                    {
                        internal::platformCloseFile({local_file_hdl});
                    }

                    break;
                }
            }
        }

        return {};
    }

    FileHdl createFile(char const * path, FileFormat fmt)
    {
        for (auto const & layer_desc : layers)
        {
            if (strStartWith(path, layer_desc.vfs_path.c_str()))
            {
                char local_file_path[LOCAL_PATH_MAX_LEN + 1];

                strCpyT(local_file_path, layer_desc.local_path.c_str());
                concatLocalPath(local_file_path, layer_desc.local_path.length(),
                                path + layer_desc.vfs_path.length());
                normalizeLocalPath(local_file_path);

                auto const local_file_hdl = internal::platformCreateFile(local_file_path, fmt);

                if (isValid(local_file_hdl))
                {
                    FileHdl file_hdl = createFileHdl(local_file_hdl);

                    if (sbExpect(isValid(file_hdl), "Out of file descriptors"))
                    {
                        ++opened_file_count;
                        return file_hdl;
                    }
                    else
                    {
                        internal::platformCloseFile({local_file_hdl});
                    }

                    break;
                }
            }
        }

        return {};
    }

private:
    using FileDescPool = ObjectPoolAllocator<FileDesc, GlobalHeapProvider>;

    FileHdl createFileHdl(internal::LayerFileHdl layer_hdl)
    {
        FileDesc * const file_desc = static_cast<FileDesc *>(file_desc_pool.allocate());

        if (sbExpect(nullptr != file_desc))
        {
            *file_desc = {layer_hdl, ++curr_file_gen};

            MemoryArena arena = file_desc_pool.getArena();
            FileDesc * const base_obj = static_cast<FileDesc *>(arena.m_ptr);

            FileHdlHelper const helper_hdl = {
                .unpacked = {numericConv<ui16>(sbstd::distance(base_obj, file_desc)),
                             numericConv<ui16>(file_desc->gen)}};

            return FileHdl{helper_hdl.packed};
        }

        return FileHdl{};
    }

    void destroyFileDesc(FileHdl hdl)
    {
        sbAssert(isValid(hdl));

        FileHdlHelper helper_hdl = {.packed = hdl.value};

        MemoryArena const arena = file_desc_pool.getArena();
        FileDesc * const file_desc = static_cast<FileDesc *>(arena.m_ptr) + helper_hdl.unpacked.hdl;

        sbAssert(arena.isInRange((void *)file_desc));
        sbAssert(helper_hdl.unpacked.gen == file_desc->gen);

        *file_desc = {};
        file_desc_pool.deallocate((void *)file_desc);
    }

    FileDescPool file_desc_pool;
    sb::StaticVector<LayerDesc, VFS_LAYER_MAX_COUNT> layers;
    FileGen curr_file_gen;
    ui16 opened_file_count;
};

} // namespace sb

static sb::VirtualFileSystemImpl * g_vfs = nullptr;

sb::b8 sb::VirtualFileSystem::initialize(InitDesc const & init)
{
    if (sbExpect(nullptr == g_vfs, "Vritual File System is already initialized"))
    {
        g_vfs = sbNew(VirtualFileSystemImpl)(init.layers);
        return true;
    }

    return false;
}

sb::b8 sb::VirtualFileSystem::terminate()
{
    if (sbExpect(nullptr != g_vfs, "Vritual File System is not initialized"))
    {
        sbDelete(g_vfs);
        g_vfs = nullptr;

        return true;
    }

    return false;
}

sb::FileHdl sb::VirtualFileSystem::openFileRead(char const * path, FileFormat fmt)
{
    sbAssert(nullptr != g_vfs);
    sbAssert(nullptr != path);
    sbWarn(isVFSPathValid(path));

    return g_vfs->openFileRead(path, fmt);
}

sb::FileHdl sb::VirtualFileSystem::openFileWrite(char const * path, FileWriteMode mode,
                                                 FileFormat fmt)
{
    sbAssert(nullptr != g_vfs);
    sbAssert(nullptr != path);
    sbWarn(isVFSPathValid(path));

    return g_vfs->openFileWrite(path, mode, fmt);
}

void sb::VirtualFileSystem::closeFile(FileHdl hdl)
{
    sbAssert(nullptr != g_vfs);
    sbAssert(isValid(hdl));

    return g_vfs->closeFile(hdl);
}

sb::FileHdl sb::VirtualFileSystem::createFile(char const * path, FileFormat fmt)
{
    sbAssert(nullptr != g_vfs);
    sbAssert(nullptr != path) sbWarn(isVFSPathValid(path));

    return g_vfs->createFile(path, fmt);
}

sb::FileSize sb::VirtualFileSystem::readFile(FileHdl hdl, sbstd::span<ui8> buffer, FileSize cnt)
{
    sbAssert((FileSize)buffer.size() >= cnt);
    sbAssert(-1 <= cnt);

    if (sbExpect(isValid(hdl)))
    {
        return g_vfs->readFile(hdl, buffer.data(), (cnt == -1) ? (FileSize)buffer.size() : cnt);
    }

    return 0;
}

sb::FileSize sb::VirtualFileSystem::getFileLength(FileHdl hdl)
{
    if (sbExpect(isValid(hdl)))
    {
        return g_vfs->getFileLength(hdl);
    }

    return 0;
}
