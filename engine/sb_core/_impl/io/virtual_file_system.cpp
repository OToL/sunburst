#include <sb_core/io/virtual_file_system.h>
#include <sb_core/io/path.h>
#include <sb_core/memory/allocator/pool_allocator.h>
#include <sb_core/memory/provider/global_heap_provider.h>
#include <sb_core/memory/memory.h>
#include <sb_core/unique_ptr.h>
#include <sb_core/memory/memory.h>
#include <sb_core/string/utility.h>
#include <sb_core/container/static_vector.h>
#include <sb_core/string/utility.h>

#include "local_file_system.h"

#include <sb_std/algorithm>
#include <sb_std/xutility>

/*
using StaticLogicalPath = sb::StaticString<sb::VFS_PATH_MAX_LEN + 1>;

// Because we tamper 16 last FileHdl bits with a generation ID
static_assert(sb::VirtualFileSystem::MAX_CONCURRENT_OPENED_FILES <= 0xFFFFU);

namespace sb::internal {

class FileSystemImpl
{
public:
    using FileGen = ui16;

    struct FileDesc
    {
        IFileSystemLayer::FileId m_id;
        IFileSystemLayer * m_layer;
        ui32 m_Gen; // Has to be last because the FileDesc allocator is using its first
                    // bytes for its internal link list
    };

    // TODO: use a loki like assoc_vector to separate m_id from the rest of the data
    struct LayerDesc
    {
        HashStr m_id;
        StaticLogicalPath m_logical_path;
        UniquePtr<IFileSystemLayer> m_layer; // TODO: Change to an id/hdl system
    };

    using FSLayers = StaticVector<LayerDesc, 15>;

    FileSystemImpl(VFS::InitParams const & init)
        : m_file_desc_gen(0)
        , m_opened_file_cnt(0)
        , m_file_desc_pool({VirtualFileSystem::MAX_CONCURRENT_OPENED_FILES})
        , m_physical_layer_pool({50U})
    {
        for (auto & layer : init.m_layers)
        {
            sbWarn(isValid(layer.m_name));

            auto const layerIter = sbstd::find_if(
                begin(m_layers), end(m_layers),
                [&layer](LayerDesc const & layer_desc) { return layer_desc.m_id == layer.m_name; });
            auto const layer_path_len = strlen(layer.m_logical_path);

            if (sbExpect((layerIter == end(m_layers)) && (0 != layer_path_len) &&
                         isVFSPathValid(layer.m_logical_path) &&
                         (layer.m_logical_path[layer_path_len - 1] == VFS_PATH_SEPARATOR)))
            {
                m_layers.push_back({.m_id = layer.m_name,
                                    .m_logical_path = layer.m_logical_path,
                                    .m_layer = sbstd::move(layer.m_layer)});
            }
        }
    }

    FileSystemImpl & operator=(FileSystemImpl const &) = delete;
    FileSystemImpl(FileSystemImpl const &) = delete;

    ~FileSystemImpl(){sbWarn(0 == m_opened_file_cnt,
                             "Not all files have been closed before destroying the File System")}

    FileSize readFile(FileHdl hdl, ui8 * buffer, FileSize cnt)
    {
        auto const & desc = getFileDescItem(hdl);
        return desc.m_layer->readFile(desc.m_id, buffer, cnt);
    }

    FileSize getFileLength(FileHdl hdl)
    {
        auto const & desc = getFileDescItem(hdl);
        return desc.m_layer->getFileLength(desc.m_id);
    }

    template <typename TFunc>
    FileHdl execFileActionOnLayers(char const * path, TFunc const & func)
    {
        FSLayers::iterator layerIter = begin(m_layers);

        do
        {
            layerIter = sbstd::find_if(layerIter, end(m_layers), [path](LayerDesc const & layer) {
                return strStartWith(path, layer.m_logical_path.c_str());
            });

            if (layerIter != end(m_layers))
            {
                auto file_hdl =
                    func(*this, *layerIter->m_layer, path + layerIter->m_logical_path.length());

                if (isValid(file_hdl))
                {
                    return file_hdl;
                }

                ++layerIter;
            }
        } while (layerIter != end(m_layers));

        return {};
    }

    FileHdl openFileWrite(char const * path, FileWriteMode mode, FileFormat fmt)
    {
        sbWarn(isVFSPathValid(path));

        return execFileActionOnLayers(
            path,
            [fmt, mode](FileSystemImpl & fs, IFileSystemLayer & layer, char const * layer_path) {
                auto layer_file_hdl = layer.openFileWrite(layer_path, mode, fmt);
                FileHdl file_hdl = {};

                if (nullptr != layer_file_hdl.value)
                {
                    file_hdl = fs.allocateFileDesc(layer_file_hdl, &layer);

                    if (sbExpect(isValid(file_hdl), "Not enough file descriptor"))
                    {
                        layer.closeFile(layer_file_hdl);
                        file_hdl = {};
                    }
                }

                return file_hdl;
            });
    }



    FileHdl createFile(char const * path, FileFormat fmt)
    {
        sbWarn(isVFSPathValid(path));

        auto const hdl = execFileActionOnLayers(
            path, [fmt](FileSystemImpl & fs, IFileSystemLayer & layer, char const * layer_path) {
                auto layer_file_hdl = layer.createFile(layer_path, fmt);
                FileHdl file_hdl = {};

                if (nullptr != layer_file_hdl.value)
                {
                    file_hdl = fs.allocateFileDesc(layer_file_hdl, &layer);

                    if (sbExpect(isValid(file_hdl), "Not enough file descriptor"))
                    {
                        layer.closeFile(layer_file_hdl);
                        file_hdl = {};
                    }
                }

                return file_hdl;
            });

        if (isValid(hdl))
        {
            ++m_opened_file_cnt;
        }

        return hdl;
    }

    char const * getLayerPhysicalPath(VFS::LayerName name)
    {
        auto const layer_desc =
            sbstd::find_if(begin(m_layers), end(m_layers),
                           [name](LayerDesc const & layer) { return layer.m_id == name; });

        if (nullptr != layer_desc)
        {
            return layer_desc->m_layer->getPhysicalPath();
        }

        return "";
    }

    void closeFile(FileHdl hdl)
    {
        sbAssert(0 < m_opened_file_cnt);

        auto & desc = getFileDescItem(hdl);
        desc.m_layer->closeFile(desc.m_id);

        deallocateFileDesc(hdl);

        --m_opened_file_cnt;
    }

private:
    union FileHdlHelper
    {
        struct
        {
            ui16 m_hdl;
            ui16 m_gen;
        } m_unpacked;

        FileHdl::ValueType m_packed;
    };

    b8 isLocalFileSystemLayer(IFileSystemLayer const * layer)
    {
        return m_physical_layer_pool.owns(layer);
    }

    FileHdl allocateFileDesc(IFileSystemLayer::FileId hdl, IFileSystemLayer * layer)
    {
        FileDesc * const file_desc = static_cast<FileDesc *>(m_file_desc_pool.allocate());

        if (sbExpect(nullptr != file_desc))
        {
            *file_desc = {hdl, layer, ++m_file_desc_gen};

            MemoryArena arena = m_file_desc_pool.getArena();
            FileDesc * const base_obj = static_cast<FileDesc *>(arena.m_ptr);

            FileHdlHelper helper_hdl;
            helper_hdl.m_unpacked = {numericConv<ui16>(sbstd::distance(base_obj, file_desc)),
                                     numericConv<ui16>(file_desc->m_Gen)};

            return FileHdl{helper_hdl.m_packed};
        }

        return FileHdl{};
    }

    void deallocateFileDesc(FileHdl hdl)
    {
        sbAssert(isValid(hdl));

        FileHdlHelper helper_hdl;
        helper_hdl.m_packed = hdl.value;

        MemoryArena arena = m_file_desc_pool.getArena();
        FileDesc * const file_desc =
            static_cast<FileDesc *>(arena.m_ptr) + helper_hdl.m_unpacked.m_hdl;

        sbAssert(arena.isInRange((void *)file_desc));
        sbAssert(helper_hdl.m_unpacked.m_gen == file_desc->m_Gen);

        zeroStruct(file_desc);
        m_file_desc_pool.deallocate((void *)file_desc);
    }

    FileDesc & getFileDescItem(FileHdl hdl)
    {
        sbAssert(isValid(hdl));

        FileHdlHelper helper_hdl;
        helper_hdl.m_packed = hdl.value;

        MemoryArena arena = m_file_desc_pool.getArena();
        FileDesc * const file_desc =
            static_cast<FileDesc *>(arena.m_ptr) + helper_hdl.m_unpacked.m_hdl;

        sbAssert(arena.isInRange((void *)file_desc));
        sbAssert(helper_hdl.m_unpacked.m_gen == file_desc->m_Gen);

        return *file_desc;
    }

    FileGen m_file_desc_gen;
    usize m_opened_file_cnt;
    FSLayers m_layers;

    using LocalFileSystemLayerPool = ObjectPoolAllocator<LocalFileSystemLayer, GlobalHeapProvider>;
    using FileDescPool = ObjectPoolAllocator<FileDesc, GlobalHeapProvider>;

    FileDescPool m_file_desc_pool;

    LocalFileSystemLayerPool m_physical_layer_pool;
};

} // namespace sb::internal


sb::FileHdl sb::VirtualFileSystem::openFileRead(char const * path, FileFormat fmt)
{
    sbAssert((nullptr != gs_file_system) && (nullptr != path));
    sbWarn(isVFSPathValid(path));

    return gs_file_system->openFileRead(path, fmt);
}

sb::FileHdl sb::VirtualFileSystem::openFileWrite(char const * path, FileWriteMode mode, FileFormat fmt)
{
    sbAssert((nullptr != gs_file_system) && (nullptr != path));
    sbWarn(isVFSPathValid(path));

    return gs_file_system->openFileWrite(path, mode, fmt);
}

sb::FileHdl sb::VirtualFileSystem::createFile(char const * path, FileFormat fmt)
{
    sbAssert((nullptr != gs_file_system) && (nullptr != path));
    sbWarn(isVFSPathValid(path));

    return gs_file_system->createFile(path, fmt);
}

char const * sb::VirtualFileSystem::getLayerPhysicalPath(LayerName name)
{
    sbAssert(nullptr != gs_file_system);

    return gs_file_system->getLayerPhysicalPath(name);
}

void sb::VirtualFileSystem::closeFile(FileHdl hdl)
{
    sbAssert(nullptr != gs_file_system);

    if (sbExpect(isValid(hdl)))
    {
        gs_file_system->closeFile(hdl);
    }
}

sb::FileSize sb::VirtualFileSystem::readFile(FileHdl hdl, sbstd::span<ui8> buffer, FileSize cnt)
{
    sbAssert((FileSize)buffer.size() >= cnt);
    sbAssert(-1 <= cnt);

    if (sbExpect(isValid(hdl)))
    {
        return gs_file_system->readFile(hdl, buffer.data(),
                                        (cnt == -1) ? (FileSize)buffer.size() : cnt);
    }

    return 0;
}

sb::FileSize sb::VirtualFileSystem::getFileLength(FileHdl hdl)
{
    if (sbExpect(isValid(hdl)))
    {
        return gs_file_system->getFileLength(hdl);
    }

    return 0;
}

sb::VirtualFileSystem::LayerPtr sb::VirtualFileSystem::createLocalFileSystemLayer(char const * local_root_path)
{
    return sbstd::static_pointer_cast<IFileSystemLayer>(
        makeUnique<LocalFileSystemLayer>(local_root_path));
}
*/

namespace sb::internal
{
    class VirtualFileSystem
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
            LayerFileHdl hdl;
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

        VirtualFileSystem(VirtualFileSystem const &) = delete;
        VirtualFileSystem(VirtualFileSystem &&) = delete;

    public:

        LayerDesc * findLayer(VFS::LayerName layer_id)
        {
            auto const layer_iter = sbstd::find_if(begin(layers), end(layers), [layer_id](LayerDesc const & layer_desc) 
            { 
                return layer_desc.id == layer_id; 
            });

            return (layer_iter == end(layers))?nullptr:layer_iter;
        }

        VirtualFileSystem(sbstd::span<VFS::LayerInitDesc> layers_desc)
            : file_desc_pool({VFS_FILE_MAX_OPENED})
            , curr_file_gen(0U)
            , opened_file_count(0U)
        {
            sbAssert(layers_desc.size() <= VFS_LAYER_MAX_COUNT);

            for (auto & layer_desc : layers_desc)
            {
                if (!(sbExpect(isValid(layer_desc.name), "VFS layer must have a valid name") &&
                    sbExpect(nullptr == findLayer(layer_desc.name), "VFS layer already registered with the same name") &&
                    sbExpect(isVFSPathValid(layer_desc.vfs_path.c_str()), "Invalid layer VFS path")))
                {
                    continue;
                }

                auto & new_layer = layers.emplace_back(layer_desc.name, layer_desc.vfs_path, layer_desc.local_path);

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

        FileHdl openFileRead(char const * path, FileFormat fmt)
        {
            sbWarn(isVFSPathValid(path));

            for (auto const & layer_desc : layers)
            {
               if (strStartWith(path, layer_desc.vfs_path.c_str()))
                {
                    char local_file_path[LOCAL_PATH_MAX_LEN + 1];

                    strCpyT(local_file_path, layer_desc.local_path.c_str());
                    concatLocalPath(local_file_path, layer_desc.local_path.length(), path + layer_desc.vfs_path.length());

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

    private:

        using FileDescPool = ObjectPoolAllocator<FileDesc, GlobalHeapProvider>;

        FileHdl createFileHdl(LayerFileHdl layer_hdl)
        {
            FileDesc * const file_desc = static_cast<FileDesc *>(file_desc_pool.allocate());

            if (sbExpect(nullptr != file_desc))
            {
                *file_desc = {layer_hdl, ++curr_file_gen};

                MemoryArena arena = file_desc_pool.getArena();
                FileDesc * const base_obj = static_cast<FileDesc *>(arena.m_ptr);

                FileHdlHelper const helper_hdl = {.unpacked = {
                                                    numericConv<ui16>(sbstd::distance(base_obj, file_desc)),
                                                    numericConv<ui16>(file_desc->gen)
                                                }};

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
}

static sb::internal::VirtualFileSystem * g_vfs = nullptr;

sb::b8 sb::VirtualFileSystem::initialize(InitDesc const & init)
{
    if (sbExpect(nullptr == g_vfs, "Vritual File System is already initialized"))
    {
        g_vfs = sbNew(internal::VirtualFileSystem)(init.layers);
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
    sbAssert((nullptr != g_vfs) && (nullptr != path));
    sbWarn(isVFSPathValid(path));

    return g_vfs->openFileRead(path, fmt);
}

void sb::VirtualFileSystem::closeFile(FileHdl hdl)
{
    sbAssert(isValid(hdl));

    return g_vfs->closeFile(hdl);
}
