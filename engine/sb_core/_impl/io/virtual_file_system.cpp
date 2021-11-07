#include "local_file_system.h"
#include <sb_core/io/virtual_file_system.h>
#include <sb_core/io/path.h>
#include <sb_core/memory/allocator/object_pool_allocator.h>
#include <sb_core/memory/allocator/global_heap_allocator.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/utility.h>
#include <sb_core/container/fix_array.h>
#include <sb_core/string/utility.h>
#include <sb_core/cast.h>
#include <sb_core/string/format.h>
#include <sb_core/log.h>

#include <sb_std/algorithm>
#include <sb_std/xutility>

namespace sb { namespace internal {

    class VirtualFileSystemImpl
    {
        struct LayerDesc
        {
            VFS::LayerName id;
            FixString<VFS_PATH_MAX_LEN + 1> vfs_path;
            FixString<SYS_PATH_MAX_LEN + 1> local_path;
        };

        using FileGen = u16;

        struct FileDesc
        {
            FileProps props;
            internal::LayerFile hdl;
            u16 gen;
        };

        union FileHelper
        {
            struct
            {
                u16 hdl;
                u16 gen;
            } unpacked;

            FileHdl::ValueType packed;
        };
        static_assert(sizeof(FileHelper) == sizeof(FileHdl::ValueType));

    public:
        VirtualFileSystemImpl(VirtualFileSystemImpl const &) = delete;
        VirtualFileSystemImpl(VirtualFileSystemImpl &&) = delete;
        VirtualFileSystemImpl & operator=(VirtualFileSystemImpl const &) = delete;
        VirtualFileSystemImpl & operator=(VirtualFileSystemImpl &&) = delete;

        LayerDesc * findLayer(VFS::LayerName layer_id)
        {
            auto const layer_iter =
                sbstd::find_if(begin(layers), end(layers),
                               [layer_id](LayerDesc const & layer_desc) { return layer_desc.id == layer_id; });

            return (layer_iter == end(layers)) ? nullptr : layer_iter;
        }

        VirtualFileSystemImpl(sbstd::span<VFS::LayerDesc> layers_desc)
            : file_desc_pool(VFS_FILE_MAX_OPENED)
            , curr_file_gen(0U)
            , opened_file_count(0U)
        {
            sb_assert(layers_desc.size() <= VFS_LAYER_MAX_COUNT);

            for (auto & layer_desc : layers_desc)
            {
                if (!(sb_expect(hash_str::isValid(layer_desc.name), "VFS layer must have a valid name") &&
                      sb_expect(nullptr == findLayer(layer_desc.name),
                                  "VFS layer already registered with the same name") &&
                      sb_expect(isVFSPathValid(layer_desc.vfs_path.c_str()), "Invalid layer VFS path")))
                {
                    continue;
                }

                auto & new_layer = layers.emplace_back(layer_desc.name, layer_desc.vfs_path, layer_desc.local_path);

                if (VFS_PATH_SEPARATOR != new_layer.vfs_path.back())
                {
                    if (!sb_expect(new_layer.vfs_path.push_back(VFS_PATH_SEPARATOR)))
                    {
                        layers.pop_back();
                        continue;
                    }
                }

                if (SYS_PATH_SEPARATOR != new_layer.local_path.back())
                {
                    if (!sb_expect(new_layer.local_path.push_back(SYS_PATH_SEPARATOR)))
                    {
                        layers.pop_back();
                        continue;
                    }
                }
            }
        }

        ~VirtualFileSystemImpl()
        {
            sb_warn(0 == opened_file_count, "Not all files have been closed before destroying the FileHdl System");
        }

        FileDesc & getFileDesc(FileHdl hdl)
        {
            sb_assert(isValid(hdl));

            FileHelper helper_hdl = {.packed = hdl.value};

            MemoryArena arena = file_desc_pool.getArena();
            FileDesc * const file_desc = static_cast<FileDesc *>(arena.data) + helper_hdl.unpacked.hdl;

            sb_assert(memory_arena::isInRange(arena, file_desc));
            sb_assert(helper_hdl.unpacked.gen == file_desc->gen);

            return *file_desc;
        }

        void closeFile(FileHdl hdl)
        {
            sb_assert(0 < opened_file_count);

            auto & desc = getFileDesc(hdl);
            internal::platformCloseFile(desc.hdl);

            destroyFileDesc(hdl);

            --opened_file_count;
        }

        FileSize readFile(FileHdl hdl, u8 * buffer, FileSize cnt)
        {
            auto const & desc = getFileDesc(hdl);

            if (sb_expect(0 != (desc.props.access.value & FileAccessFlags::READ)))
            {
                return internal::platformReadFile(desc.hdl, buffer, cnt);
            }

            return 0;
        }

        FileSize writeFile(FileHdl hdl, u8 const * buffer, FileSize cnt)
        {
            auto const & desc = getFileDesc(hdl);

            if (sb_expect(0 != (desc.props.access.value & FileAccessFlags::WRITE)))
            {
                return internal::platformWriteFile(desc.hdl, buffer, cnt);
            }

            return 0;
        }

        FileSize getFileLength(FileHdl hdl)
        {
            auto const & desc = getFileDesc(hdl);
            return internal::platformFileLength(desc.hdl);
        }

        FileProps getFileProps(FileHdl hdl)
        {
            auto const & desc = getFileDesc(hdl);
            return desc.props;
        }

        b8 fileExists(char const * path)
        {
            for (auto const & layer_desc : layers)
            {
                if (strStartsWith(path, layer_desc.vfs_path.c_str()))
                {
                    char local_file_path[SYS_PATH_MAX_LEN + 1];
                    buildLocalFilePath(local_file_path, layer_desc, path);

                    if (internal::platformFileExists(&local_file_path[0]))
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        static void buildLocalFilePath(char (&local_file_path)[SYS_PATH_MAX_LEN + 1], LayerDesc const & layer_desc,
                                       char const * file_path)
        {
            strCpyT(local_file_path, layer_desc.local_path.c_str());
            concatSysPath(local_file_path, layer_desc.local_path.length(), file_path + layer_desc.vfs_path.length());
            normalizeSysPath(&local_file_path[0]);
        }

        FileHdl openFileRead(char const * path, FileFormat fmt)
        {
            for (auto const & layer_desc : layers)
            {
                if (strStartsWith(path, layer_desc.vfs_path.c_str()))
                {
                    char local_file_path[SYS_PATH_MAX_LEN + 1];
                    buildLocalFilePath(local_file_path, layer_desc, path);

                    auto const local_file_hdl = internal::platformOpenFileRead(&local_file_path[0], fmt);

                    if (layerfile_isValid(local_file_hdl))
                    {
                        FileProps const file_props = {.fmt = fmt, .access = FileAccessFlags::READ};
                        FileHdl file_hdl = createFile(local_file_hdl, file_props);

                        if (sb_expect(isValid(file_hdl), "Out of file descriptors"))
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

        FileHdl openFileReadWrite(char const * path, FileWriteMode mode, FileFormat fmt)
        {
            for (auto const & layer_desc : layers)
            {
                if (strStartsWith(path, layer_desc.vfs_path.c_str()))
                {
                    char local_file_path[SYS_PATH_MAX_LEN + 1];
                    buildLocalFilePath(local_file_path, layer_desc, path);

                    auto const local_file_hdl = internal::platformOpenFileReadWrite(&local_file_path[0], mode, fmt);

                    if (layerfile_isValid(local_file_hdl))
                    {
                        FileProps const file_props = {.fmt = fmt,
                                                      .access = FileAccessFlags::READ | FileAccessFlags::WRITE};
                        FileHdl file_hdl = createFile(local_file_hdl, file_props);

                        if (sb_expect(isValid(file_hdl), "Out of file descriptors"))
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
                if (strStartsWith(path, layer_desc.vfs_path.c_str()))
                {
                    char local_file_path[SYS_PATH_MAX_LEN + 1];
                    buildLocalFilePath(local_file_path, layer_desc, path);

                    auto const local_file_hdl = internal::platformOpenFileWrite(&local_file_path[0], mode, fmt);

                    if (layerfile_isValid(local_file_hdl))
                    {
                        FileProps const file_props = {.fmt = fmt, .access = FileAccessFlags::WRITE};
                        FileHdl file_hdl = createFile(local_file_hdl, file_props);

                        if (sb_expect(isValid(file_hdl), "Out of file descriptors"))
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

        FileHdl createFileReadWrite(char const * path, FileFormat fmt)
        {
            for (auto const & layer_desc : layers)
            {
                if (strStartsWith(path, layer_desc.vfs_path.c_str()))
                {
                    char local_file_path[SYS_PATH_MAX_LEN + 1];
                    buildLocalFilePath(local_file_path, layer_desc, path);

                    auto const local_file_hdl = internal::platformCreateFileReadWrite(&local_file_path[0], fmt);

                    if (layerfile_isValid(local_file_hdl))
                    {
                        FileProps const file_props = {.fmt = fmt,
                                                      .access = FileAccessFlags::READ | FileAccessFlags::WRITE};
                        FileHdl file_hdl = createFile(local_file_hdl, file_props);

                        if (sb_expect(isValid(file_hdl), "Out of file descriptors"))
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

        FileHdl createFileWrite(char const * path, FileFormat fmt)
        {
            for (auto const & layer_desc : layers)
            {
                if (strStartsWith(path, layer_desc.vfs_path.c_str()))
                {
                    char local_file_path[SYS_PATH_MAX_LEN + 1];
                    buildLocalFilePath(local_file_path, layer_desc, path);

                    auto const local_file_hdl = internal::platformCreateFileWrite(&local_file_path[0], fmt);

                    if (layerfile_isValid(local_file_hdl))
                    {
                        FileProps const file_props = {.fmt = fmt, .access = FileAccessFlags::WRITE};
                        FileHdl file_hdl = createFile(local_file_hdl, file_props);

                        if (sb_expect(isValid(file_hdl), "Out of file descriptors"))
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
        using FileDescPool = ObjectPoolAllocator<FileDesc, GlobalHeapAllocator>;

        FileHdl createFile(internal::LayerFile layer_hdl, FileProps props)
        {
            FileDesc * const file_desc = static_cast<FileDesc *>(file_desc_pool.allocate().data);

            if (sb_expect(nullptr != file_desc))
            {
                *file_desc = {props, layer_hdl, ++curr_file_gen};

                MemoryArena arena = file_desc_pool.getArena();
                auto const base_obj = static_cast<FileDesc *>(arena.data);

                FileHelper const helper_hdl = {.unpacked = {integral_cast<u16>(sbstd::distance(base_obj, file_desc)),
                                                            integral_cast<u16>(file_desc->gen)}};

                return FileHdl{helper_hdl.packed};
            }

            return FileHdl{};
        }

        void destroyFileDesc(FileHdl hdl)
        {
            sb_assert(isValid(hdl));

            FileHelper helper_hdl = {.packed = hdl.value};

            MemoryArena const arena = file_desc_pool.getArena();
            FileDesc * const file_desc = static_cast<FileDesc *>(arena.data) + helper_hdl.unpacked.hdl;

            sb_assert(memory_arena::isInRange(arena, (void *)file_desc));
            sb_assert(helper_hdl.unpacked.gen == file_desc->gen);

            *file_desc = {};
            file_desc_pool.deallocate((void *)file_desc);
        }

        FileDescPool file_desc_pool;
        sb::FArray<LayerDesc, VFS_LAYER_MAX_COUNT> layers;
        FileGen curr_file_gen;
        u16 opened_file_count;
    };
}} // namespace sb::internal

static sb::internal::VirtualFileSystemImpl * g_vfs = nullptr;

sb::b8 sb::VirtualFileSystem::initialize(InitDesc const & init)
{
    if (sb_expect(nullptr == g_vfs, "Vritual FileHdl System is already initialized"))
    {
        g_vfs = sb_new(GHEAP, internal::VirtualFileSystemImpl, init.layers);
        return true;
    }

    return false;
}

sb::b8 sb::VirtualFileSystem::terminate()
{
    if (sb_expect(nullptr != g_vfs, "Vritual FileHdl System is not initialized"))
    {
        sb_delete(GHEAP, g_vfs);
        g_vfs = nullptr;

        return true;
    }

    return false;
}

sb::FileHdl sb::VirtualFileSystem::openFile(char const * path, FileWriteMode mode, FileFormat fmt,
                                         bool create_if_not_exist)
{
    sb_assert(nullptr != g_vfs);
    sb_assert(nullptr != path);
    sb_warn(isVFSPathValid(path));

    FileHdl file_hdl = g_vfs->openFileReadWrite(path, mode, fmt);

    if (create_if_not_exist && !isValid(file_hdl))
    {
        file_hdl = createFile(path, fmt);
    }

    return file_hdl;
}

sb::FileHdl sb::VirtualFileSystem::openFileWrite(char const * path, FileWriteMode mode, FileFormat fmt,
                                              bool create_if_not_exist)
{
    sb_assert(nullptr != g_vfs);
    sb_assert(nullptr != path);
    sb_warn(isVFSPathValid(path));

    FileHdl file_hdl = g_vfs->openFileWrite(path, mode, fmt);

    if (create_if_not_exist && !isValid(file_hdl))
    {
        file_hdl = createFileWrite(path, fmt);
    }

    return file_hdl;
}

sb::FileHdl sb::VirtualFileSystem::openFileRead(char const * path, FileFormat fmt)
{
    sb_assert(nullptr != g_vfs);
    sb_assert(nullptr != path);
    sb_warn(isVFSPathValid(path));

    return g_vfs->openFileRead(path, fmt);
}

void sb::VirtualFileSystem::closeFile(FileHdl hdl)
{
    sb_assert(nullptr != g_vfs);
    sb_assert(isValid(hdl));

    return g_vfs->closeFile(hdl);
}

sb::FileHdl sb::VirtualFileSystem::createFileWrite(char const * path, FileFormat fmt)
{
    sb_assert(nullptr != g_vfs);
    sb_assert(nullptr != path) sb_warn(isVFSPathValid(path));

    return g_vfs->createFileWrite(path, fmt);
}

sb::FileHdl sb::VirtualFileSystem::createFile(char const * path, FileFormat fmt)
{
    sb_assert(nullptr != g_vfs);
    sb_assert(nullptr != path) sb_warn(isVFSPathValid(path));

    return g_vfs->createFileReadWrite(path, fmt);
}

sb::FileSize sb::VirtualFileSystem::readFile(FileHdl hdl, sbstd::span<u8> buffer, FileSize cnt)
{
    sb_assert(nullptr != g_vfs);
    sb_assert(-1 <= cnt);
    sb_assert(integral_cast<FileSize>(buffer.size()) >= cnt);

    if (sb_expect(isValid(hdl)))
    {
        return g_vfs->readFile(hdl, buffer.data(), (cnt == -1) ? (FileSize)buffer.size() : cnt);
    }

    return 0;
}

sb::FileSize sb::VirtualFileSystem::writeFile(FileHdl hdl, sbstd::span<u8 const> buffer, FileSize cnt)
{
    sb_assert(nullptr != g_vfs);
    sb_assert(-1 <= cnt);
    sb_assert(integral_cast<FileSize>(buffer.size()) >= cnt);

    if (sb_expect(isValid(hdl)))
    {
        return g_vfs->writeFile(hdl, buffer.data(), (cnt == -1) ? (FileSize)buffer.size() : cnt);
    }

    return 0;
}

sb::FileSize sb::VirtualFileSystem::getFileLength(FileHdl hdl)
{
    sb_assert(nullptr != g_vfs);

    if (sb_expect(isValid(hdl)))
    {
        return g_vfs->getFileLength(hdl);
    }

    return 0;
}

sb::FileProps sb::VirtualFileSystem::getFileProps(FileHdl hdl)
{
    sb_assert(nullptr != g_vfs);

    if (sb_expect(isValid(hdl)))
    {
        return g_vfs->getFileProps(hdl);
    }

    return {};
}

sb::b8 sb::VirtualFileSystem::fileExists(char const * path)
{
    sb_assert(nullptr != g_vfs);

    return g_vfs->fileExists(path);
}

sbstd::span<sb::u8> sb::VirtualFileSystem::readFile(char const * path, IAllocator & alloc, FileFormat fmt)
{
    auto const fd = openFileRead(path, fmt);
    if (!isValid(fd))
    {
        sb_log_warning("Failed to open '{}'", path);
        return {};
    }

    auto const file_size = getFileLength(fd);
    if (0 >= file_size)
    {
        sb_log_warning("Failed to read '{}' content", path);
        closeFile(fd);
        return {};
    }

    sb::MemoryArena const arena = alloc.allocate(integral_cast<usize>(file_size));
    if (!memory_arena::isValid(arena))
    {
        sb_log_warning("Failed to allocate memory to back '{}' content", path);
        closeFile(fd);
        return {};
    }

    sbstd::span<u8> const file_content((u8 *)arena.data, arena.size);
    auto const read_cnt = readFile(fd, file_content);
    sb_warn(integral_cast<usize>(read_cnt) == file_content.size());

    closeFile(fd);

    return file_content;
}
