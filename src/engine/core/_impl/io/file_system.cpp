#include <core/io/file_system.h>
#include <core/io/file_system_layer.h>
#include <core/io/path.h>
#include <core/memory/allocator/pool_allocator.h>
#include <core/memory/provider/global_heap_provider.h>
#include <core/memory/memory.h>
#include <core/unique_ptr.h>
#include <core/memory/memory.h>
#include <core/string/static_string.h>
#include <core/container/static_vector.h>
#include <core/string/utility.h>

#include "file_system_platform.h"
#include "local_file_system_layer.h"

#include <libc++/algorithm>
#include <libc++/array>
#include <libc++/utility>
#include <libc++/memory>

namespace sb {

using StaticLogicalPath = StaticString<LPath::MAX_LEN + 1>;

// Because we tamper 16 last FileHdl bits with a generation ID
static_assert(FileSystem::MAX_CONCURRENT_OPENED_FILES <= 0xFFFFU);

class FileSystemImpl
{
    sbCopyProtect(FileSystemImpl);

public:

    using FileGen = ui16;

    struct FileDesc
    {
        IFileSystemLayer::FileId m_id;
        IFileSystemLayer * m_layer;
        ui32 m_Gen; // Has to be last because the FileDesc allocator is using its first bytes for its internal link list
    };

    // TODO: use a loki like assoc_vector to separate m_id from the rest of the data
    struct LayerDesc
    {
        HashStr m_id;
        StaticLogicalPath m_logical_path;
        UniquePtr<IFileSystemLayer> m_layer; // TODO: Change to an id/hdl system
    };

    using FSLayers = StaticVector<LayerDesc, 15>;

    FileSystemImpl(FS::InitParams const & init)
        : m_file_desc_gen(0)
        , m_opened_file_cnt(0)
        , m_file_desc_pool({FileSystem::MAX_CONCURRENT_OPENED_FILES})
        , m_physical_layer_pool({50U})
    {
        for (auto & layer : init.m_layers)
        {
            sbWarn(!layer.m_name.isNull());

            auto const layerIter = wstd::find_if(begin(m_layers), end(m_layers), [&layer](LayerDesc const & layer_desc) { return layer_desc.m_id == layer.m_name; });
            auto const layer_path_len = strlen(layer.m_logical_path);

            if (sbExpectTrue((layerIter == end(m_layers)) &&
                    (0 != layer_path_len) &&
                    LPath::isValid(layer.m_logical_path) &&
                    (layer.m_logical_path[layer_path_len - 1] == *LPath::SEPARATOR))
                )
            {
                auto & new_layer = m_layers.emplace_back();

                new_layer.m_id = layer.m_name;
                new_layer.m_layer = wstd::move(layer.m_layer);
                new_layer.m_logical_path = layer.m_logical_path;
            }
        }
    }

    ~FileSystemImpl()
    {
        sbWarn(0 == m_opened_file_cnt, "Not all files have been closed before destroying the File System")
    }

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
            layerIter = wstd::find_if(layerIter, end(m_layers), [path](LayerDesc const & layer) { return strStartWith(path, layer.m_logical_path.c_str()); });

            if (layerIter != end(m_layers))
            {
                auto file_hdl = func(*this, *layerIter->m_layer, path + layerIter->m_logical_path.length());

                if (!file_hdl.isNull())
                {
                    return file_hdl;
                }

                ++layerIter;
            }
        }
        while (layerIter != end(m_layers));

        return {};
    }

    FileHdl openFileWrite(char const * path, FileWriteMode mode, FileFormat fmt)
    {
        sbWarn(LPath::isValid(path));

        return execFileActionOnLayers(path, [fmt, mode] (FileSystemImpl &fs, IFileSystemLayer &layer, char const * layer_path) 
            {
                auto layer_file_hdl = layer.openFileWrite(layer_path, mode, fmt);
                FileHdl file_hdl;

                if (nullptr != layer_file_hdl.m_value)
                {
                    file_hdl = fs.allocateFileDesc(layer_file_hdl, &layer);

                    if (sbExpectFalse(file_hdl.isNull(), "Not enough file descriptor"))
                    {
                        layer.closeFile(layer_file_hdl);
                        file_hdl.reset();
                    }
                }

                return file_hdl;
            });
    }

    FileHdl openFileRead(char const * path, FileFormat fmt)
    {
        sbWarn(LPath::isValid(path));

        auto const hdl = execFileActionOnLayers(path, [fmt] (FileSystemImpl &fs, IFileSystemLayer &layer, char const * layer_path) 
            {
                auto layer_file_hdl = layer.openFileRead(layer_path, fmt);
                FileHdl file_hdl;

                if (nullptr != layer_file_hdl.m_value)
                {
                    file_hdl = fs.allocateFileDesc(layer_file_hdl, &layer);

                    if (sbExpectFalse(file_hdl.isNull(), "Not enough file descriptor"))
                    {
                        layer.closeFile(layer_file_hdl);
                        file_hdl.reset();
                    }
                }

                return file_hdl;
            });

        if (!hdl.isNull())
        {
            ++m_opened_file_cnt;
        }

        return hdl;
    }

    FileHdl createFile(char const * path, FileFormat fmt)
    {
        sbWarn(LPath::isValid(path));

        auto const hdl = execFileActionOnLayers(path, [fmt] (FileSystemImpl &fs, IFileSystemLayer &layer, char const * layer_path) 
            {
                auto layer_file_hdl = layer.createFile(layer_path, fmt);
                FileHdl file_hdl;

                if (nullptr != layer_file_hdl.m_value)
                {
                    file_hdl = fs.allocateFileDesc(layer_file_hdl, &layer);

                    if (sbExpectFalse(file_hdl.isNull(), "Not enough file descriptor"))
                    {
                        layer.closeFile(layer_file_hdl);
                        file_hdl.reset();
                    }
                }

                return file_hdl;
            });

        if (!hdl.isNull())
        {
            ++m_opened_file_cnt;
        }

        return hdl;
    }

    char const * getLayerPhysicalPath(FS::LayerName name)
    {
        auto const layer_desc = wstd::find_if(begin(m_layers), end(m_layers), [name](LayerDesc const & layer) { return layer.m_id == name; });

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
        struct {
            ui16 m_hdl;
            ui16 m_gen;
        } m_unpacked;

        FileHdl::StorageType m_packed;
    };

    b8 isLocalFileSystemLayer(IFileSystemLayer const * layer)
    {
        return m_physical_layer_pool.owns(layer);
    }

    FileHdl allocateFileDesc(IFileSystemLayer::FileId hdl, IFileSystemLayer * layer)
    {
        FileDesc * const file_desc = static_cast<FileDesc *>(m_file_desc_pool.allocate());

        if (sbExpectTrue(nullptr != file_desc))
        {
            *file_desc = {hdl, layer, ++m_file_desc_gen};

            MemoryArena arena = m_file_desc_pool.getArena();
            FileDesc * const base_obj = static_cast<FileDesc *>(arena.m_ptr);

            FileHdlHelper helper_hdl;
            helper_hdl.m_unpacked = {numericCast<ui16>(wstd::distance(base_obj, file_desc)), numericCast<ui16>(file_desc->m_Gen)};

            return FileHdl{helper_hdl.m_packed};
        }

        return FileHdl{};
    }

    void deallocateFileDesc(FileHdl hdl)
    {
        sbAssert(!hdl.isNull());

        FileHdlHelper helper_hdl;
        helper_hdl.m_packed = hdl.get();

        MemoryArena arena = m_file_desc_pool.getArena();
        FileDesc * const file_desc = static_cast<FileDesc *>(arena.m_ptr) + helper_hdl.m_unpacked.m_hdl;

        sbAssert(arena.isInRange((void *)file_desc));
        sbAssert(helper_hdl.m_unpacked.m_gen == file_desc->m_Gen);

        zeroStruct(file_desc);
        m_file_desc_pool.deallocate((void *)file_desc);
    }

    FileDesc & getFileDescItem(FileHdl hdl)
    {
        sbAssert(!hdl.isNull());

        FileHdlHelper helper_hdl;
        helper_hdl.m_packed = hdl.get();

        MemoryArena arena = m_file_desc_pool.getArena();
        FileDesc * const file_desc = static_cast<FileDesc *>(arena.m_ptr) + helper_hdl.m_unpacked.m_hdl;

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

static UniquePtr<FileSystemImpl> gs_file_system;

b8 FileSystem::initialize(InitParams const & init)
{
    if (sbExpectTrue(nullptr == gs_file_system, "File System already initialized"))
    {
        gs_file_system = makeUnique<FileSystemImpl>(init);

        return true;
    }

    return false;
}

b8 FileSystem::terminate()
{
    if (sbExpectTrue(nullptr != gs_file_system, "File System not initialized"))
    {
        gs_file_system = nullptr;

        return true;
    }

    return false;
}

FileHdl FileSystem::openFileRead(char const * path, FileFormat fmt)
{
    sbAssert((nullptr != gs_file_system) && (nullptr != path));
    sbWarn(LPath::isValid(path));

    return gs_file_system->openFileRead(path, fmt);
}

FileHdl FileSystem::openFileWrite(char const * path, FileWriteMode mode, FileFormat fmt)
{
    sbAssert((nullptr != gs_file_system) && (nullptr != path));
    sbWarn(LPath::isValid(path));

    return gs_file_system->openFileWrite(path, mode, fmt);
}

FileHdl FileSystem::createFile(char const * path, FileFormat fmt)
{
    sbAssert((nullptr != gs_file_system) && (nullptr != path));
    sbWarn(LPath::isValid(path));

    return gs_file_system->createFile(path, fmt);
}

char const * FileSystem::getLayerPhysicalPath(LayerName name)
{
    sbAssert(nullptr != gs_file_system);

    return gs_file_system->getLayerPhysicalPath(name);
}

void FileSystem::closeFile(FileHdl hdl)
{
    sbAssert(nullptr != gs_file_system);

    if (sbExpectTrue(!hdl.isNull()))
    {
        gs_file_system->closeFile(hdl);
    }
}

FileSize FileSystem::readFile(FileHdl hdl, wstd::span<ui8> buffer, FileSize cnt)
{
    sbAssert((FileSize)buffer.size() >= cnt);
    sbAssert(-1 <= cnt);

    if (sbExpectTrue(!hdl.isNull()))
    {
        return gs_file_system->readFile(hdl, buffer.data(), (cnt == -1) ? (FileSize)buffer.size():cnt);
    }

    return 0;
}

FileSize FileSystem::getFileLength(FileHdl hdl)
{
    if (sbExpectTrue(!hdl.isNull()))
    {
        return gs_file_system->getFileLength(hdl);
    }

    return 0;
}

FileSystem::LayerPtr FileSystem::createLocalFileSystemLayer(char const * local_root_path)
{
    return wstd::static_pointer_cast<IFileSystemLayer>(makeUnique<LocalFileSystemLayer>(local_root_path));
}

} // namespace sb
