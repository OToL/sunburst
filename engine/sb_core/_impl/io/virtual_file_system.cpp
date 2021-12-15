#include "local_file_system.h"
#include <sb_core/io/virtual_file_system.h>
#include <sb_core/io/io.h>
#include <sb_core/io/path.h>
#include <sb_core/core.h>
#include <sb_core/cast.h>
#include <sb_core/log.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/global_heap.h>

namespace sb { namespace internal {

    class VirtualFileSystem
    {
        //    struct LayerDesc
        //    {
        //        VFS::LayerName id;
        //        FixString<VFS_PATH_MAX_LEN + 1> vfs_path;
        //        FixString<SYS_PATH_MAX_LEN + 1> local_path;
        //    };

        using FileGen = u16;

        //    struct FileDesc
        //    {
        //        FileProps props;
        //        internal::LayerFile hdl;
        //        u16 gen;
        //    };

        //    union FileHelper
        //    {
        //        struct
        //        {
        //            u16 hdl;
        //            u16 gen;
        //        } unpacked;

        //        FileHdl::ValueType packed;
        //    };
        //    static_assert(sizeof(FileHelper) == sizeof(FileHdl::ValueType));

    public:
        sb_copy_protect(VirtualFileSystem);

        //    LayerDesc * findLayer(VFS::LayerName layer_id)
        //    {
        //        auto const layer_iter =
        //            sbstd::find_if(begin(layers), end(layers),
        //                           [layer_id](LayerDesc const & layer_desc) { return layer_desc.id == layer_id; });

        //        return (layer_iter == end(layers)) ? nullptr : layer_iter;
        //    }

        VirtualFileSystem()
            : _curr_file_gen(0U)
            , _opened_file_count(0U)
        {
        }

        ~VirtualFileSystem()
        {
            sb_warn(0 == _opened_file_count, "Not all files have been closed before destroying the File System");
        }

        //    FileDesc & getFileDesc(FileHdl hdl)
        //    {
        //        sb_assert(isValid(hdl));

        //        FileHelper helper_hdl = {.packed = hdl.value};

        //        MemoryArena arena = file_desc_pool.getArena();
        //        FileDesc * const file_desc = static_cast<FileDesc *>(arena.data) + helper_hdl.unpacked.hdl;

        //        sb_assert(memory_arena::isInRange(arena, file_desc));
        //        sb_assert(helper_hdl.unpacked.gen == file_desc->gen);

        //        return *file_desc;
        //    }

        void closeFile(FileHdl /*hdl*/)
        {
            //        sb_assert(0 < opened_file_count);

            //        auto & desc = getFileDesc(hdl);
            //        internal::platformCloseFile(desc.hdl);

            //        destroyFileDesc(hdl);

            //        --opened_file_count;
        }

        FileSize readFile(FileHdl /*hdl*/, u8 * /*buffer*/, FileSize /*cnt*/)
        {
            //        auto const & desc = getFileDesc(hdl);

            //        if (sb_expect(0 != (desc.props.access.value & FileAccessFlags::READ)))
            //        {
            //            return internal::platformReadFile(desc.hdl, buffer, cnt);
            //        }

            return 0;
        }

        FileSize writeFile(FileHdl /*hdl*/, u8 const * /*buffer*/, FileSize /*cnt*/)
        {
            //        auto const & desc = getFileDesc(hdl);

            //        if (sb_expect(0 != (desc.props.access.value & FileAccessFlags::WRITE)))
            //        {
            //            return internal::platformWriteFile(desc.hdl, buffer, cnt);
            //        }

            return 0;
        }

        FileSize getFileLength(FileHdl /*hdl*/)
        {
            //        auto const & desc = getFileDesc(hdl);
            //        return internal::platformFileLength(desc.hdl);

            return 0;
        }

        FileProps getFileProps(FileHdl /*hdl*/)
        {
            //        auto const & desc = getFileDesc(hdl);
            //        return desc.props;

            return {};
        }

        b8 fileExists(char const * /*path*/)
        {
            //        for (auto const & layer_desc : layers)
            //        {
            //            if (strStartsWith(path, layer_desc.vfs_path.c_str()))
            //            {
            //                char local_file_path[SYS_PATH_MAX_LEN + 1];
            //                buildLocalFilePath(local_file_path, layer_desc, path);

            //                if (internal::platformFileExists(&local_file_path[0]))
            //                {
            //                    return true;
            //                }
            //            }
            //        }

            return false;
        }


        //    static void buildLocalFilePath(char (&local_file_path)[SYS_PATH_MAX_LEN + 1], LayerDesc const &
        //    layer_desc,
        //                                   char const * file_path)
        //    {
        //        strCpyT(local_file_path, layer_desc.local_path.c_str());
        //        concatSysPath(local_file_path, layer_desc.local_path.length(), file_path +
        //        layer_desc.vfs_path.length()); normalizeSysPath(&local_file_path[0]);
        //    }

        FileHdl openFileRead(char const * /*path*/, FileFormat /*fmt*/)
        {
            //        for (auto const & layer_desc : layers)
            //        {
            //            if (strStartsWith(path, layer_desc.vfs_path.c_str()))
            //            {
            //                char local_file_path[SYS_PATH_MAX_LEN + 1];
            //                buildLocalFilePath(local_file_path, layer_desc, path);

            //                auto const local_file_hdl = internal::platformOpenFileRead(&local_file_path[0], fmt);

            //                if (layerfile_isValid(local_file_hdl))
            //                {
            //                    FileProps const file_props = {.fmt = fmt, .access = FileAccessFlags::READ};
            //                    FileHdl file_hdl = createFile(local_file_hdl, file_props);

            //                    if (sb_expect(isValid(file_hdl), "Out of file descriptors"))
            //                    {
            //                        ++opened_file_count;
            //                        return file_hdl;
            //                    }
            //                    else
            //                    {
            //                        internal::platformCloseFile({local_file_hdl});
            //                    }

            //                    break;
            //                }
            //            }
            //        }

            return {};
        }

        FileHdl openFileReadWrite(char const * /*path*/, FileWriteMode /*mode*/, FileFormat /*fmt*/)
        {
            //        for (auto const & layer_desc : layers)
            //        {
            //            if (strStartsWith(path, layer_desc.vfs_path.c_str()))
            //            {
            //                char local_file_path[SYS_PATH_MAX_LEN + 1];
            //                buildLocalFilePath(local_file_path, layer_desc, path);

            //                auto const local_file_hdl = internal::platformOpenFileReadWrite(&local_file_path[0], mode,
            //                fmt);

            //                if (layerfile_isValid(local_file_hdl))
            //                {
            //                    FileProps const file_props = {.fmt = fmt,
            //                                                  .access = FileAccessFlags::READ |
            //                                                  FileAccessFlags::WRITE};
            //                    FileHdl file_hdl = createFile(local_file_hdl, file_props);

            //                    if (sb_expect(isValid(file_hdl), "Out of file descriptors"))
            //                    {
            //                        ++opened_file_count;
            //                        return file_hdl;
            //                    }
            //                    else
            //                    {
            //                        internal::platformCloseFile({local_file_hdl});
            //                    }

            //                    break;
            //                }
            //            }
            //        }

            return {};
        }

        FileHdl openFileWrite(char const * /*path*/, FileWriteMode /*mode*/, FileFormat /*fmt*/)
        {
            //        for (auto const & layer_desc : layers)
            //        {
            //            if (strStartsWith(path, layer_desc.vfs_path.c_str()))
            //            {
            //                char local_file_path[SYS_PATH_MAX_LEN + 1];
            //                buildLocalFilePath(local_file_path, layer_desc, path);

            //                auto const local_file_hdl = internal::platformOpenFileWrite(&local_file_path[0], mode,
            //                fmt);

            //                if (layerfile_isValid(local_file_hdl))
            //                {
            //                    FileProps const file_props = {.fmt = fmt, .access = FileAccessFlags::WRITE};
            //                    FileHdl file_hdl = createFile(local_file_hdl, file_props);

            //                    if (sb_expect(isValid(file_hdl), "Out of file descriptors"))
            //                    {
            //                        ++opened_file_count;
            //                        return file_hdl;
            //                    }
            //                    else
            //                    {
            //                        internal::platformCloseFile({local_file_hdl});
            //                    }

            //                    break;
            //                }
            //            }
            //        }

            return {};
        }

        FileHdl createFileReadWrite(char const * /*path*/, FileFormat /*fmt*/)
        {
            //        for (auto const & layer_desc : layers)
            //        {
            //            if (strStartsWith(path, layer_desc.vfs_path.c_str()))
            //            {
            //                char local_file_path[SYS_PATH_MAX_LEN + 1];
            //                buildLocalFilePath(local_file_path, layer_desc, path);

            //                auto const local_file_hdl = internal::platformCreateFileReadWrite(&local_file_path[0],
            //                fmt);

            //                if (layerfile_isValid(local_file_hdl))
            //                {
            //                    FileProps const file_props = {.fmt = fmt,
            //                                                  .access = FileAccessFlags::READ |
            //                                                  FileAccessFlags::WRITE};
            //                    FileHdl file_hdl = createFile(local_file_hdl, file_props);

            //                    if (sb_expect(isValid(file_hdl), "Out of file descriptors"))
            //                    {
            //                        ++opened_file_count;
            //                        return file_hdl;
            //                    }
            //                    else
            //                    {
            //                        internal::platformCloseFile({local_file_hdl});
            //                    }

            //                    break;
            //                }
            //            }
            //        }

            return {};
        }

        FileHdl createFileWrite(char const * /*path*/, FileFormat /*fmt*/)
        {
            //        for (auto const & layer_desc : layers)
            //        {
            //            if (strStartsWith(path, layer_desc.vfs_path.c_str()))
            //            {
            //                char local_file_path[SYS_PATH_MAX_LEN + 1];
            //                buildLocalFilePath(local_file_path, layer_desc, path);

            //                auto const local_file_hdl = internal::platformCreateFileWrite(&local_file_path[0], fmt);

            //                if (layerfile_isValid(local_file_hdl))
            //                {
            //                    FileProps const file_props = {.fmt = fmt, .access = FileAccessFlags::WRITE};
            //                    FileHdl file_hdl = createFile(local_file_hdl, file_props);

            //                    if (sb_expect(isValid(file_hdl), "Out of file descriptors"))
            //                    {
            //                        ++opened_file_count;
            //                        return file_hdl;
            //                    }
            //                    else
            //                    {
            //                        internal::platformCloseFile({local_file_hdl});
            //                    }

            //                    break;
            //                }
            //            }
            //        }

            return {};
        }

        // private:
        //    using FileDescPool = ObjectPoolAllocator<FileDesc, GlobalHeapAllocator>;

        //    FileHdl createFile(internal::LayerFile layer_hdl, FileProps props)
        //    {
        //        FileDesc * const file_desc = static_cast<FileDesc *>(file_desc_pool.allocate().data);

        //        if (sb_expect(nullptr != file_desc))
        //        {
        //            *file_desc = {props, layer_hdl, ++curr_file_gen};

        //            MemoryArena arena = file_desc_pool.getArena();
        //            auto const base_obj = static_cast<FileDesc *>(arena.data);

        //            FileHelper const helper_hdl = {.unpacked = {integral_cast<u16>(sbstd::distance(base_obj,
        //            file_desc)),
        //                                                        integral_cast<u16>(file_desc->gen)}};

        //            return FileHdl{helper_hdl.packed};
        //        }

        //        return FileHdl{};
        //    }

        //    void destroyFileDesc(FileHdl hdl)
        //    {
        //        sb_assert(isValid(hdl));

        //        FileHelper helper_hdl = {.packed = hdl.value};

        //        MemoryArena const arena = file_desc_pool.getArena();
        //        FileDesc * const file_desc = static_cast<FileDesc *>(arena.data) + helper_hdl.unpacked.hdl;

        //        sb_assert(memory_arena::isInRange(arena, (void *)file_desc));
        //        sb_assert(helper_hdl.unpacked.gen == file_desc->gen);

        //        *file_desc = {};
        //        file_desc_pool.deallocate((void *)file_desc);
        //    }

        //    FileDescPool file_desc_pool;
        //    sb::DFArray<LayerDesc, VFS_LAYER_MAX_COUNT> layers;
        FileGen _curr_file_gen;
        u16 _opened_file_count;
    };

    VirtualFileSystem * g_vfs = nullptr;

}} // namespace sb::internal

sb::b8 sb::virtual_file_system::initialize(InitParams const & /*init*/)
{
    if (sb_expect(nullptr == internal::g_vfs, "Virtual File System is already initialized"))
    {
        internal::g_vfs = sb_new(GHEAP, internal::VirtualFileSystem);
        return true;
    }

    return false;
}

sb::b8 sb::virtual_file_system::terminate()
{
    if (sb_expect(nullptr != internal::g_vfs, "Virtual File System is not initialized"))
    {
        sb_delete(GHEAP, internal::g_vfs);
        internal::g_vfs = nullptr;

        return true;
    }

    return false;
}

sb::FileHdl sb::virtual_file_system::openFile(char const * path, FileWriteMode mode, FileFormat fmt,
                                              bool create_if_not_exist)
{
    sb_assert(nullptr != internal::g_vfs);
    sb_assert(nullptr != path);
    sb_warn(isVFSPathValid(path));

    FileHdl file_hdl = internal::g_vfs->openFileReadWrite(path, mode, fmt);

    if (create_if_not_exist && !isValid(file_hdl))
    {
        file_hdl = createFile(path, fmt);
    }

    return file_hdl;
}

sb::FileHdl sb::virtual_file_system::openFileWrite(char const * path, FileWriteMode mode, FileFormat fmt,
                                                   bool create_if_not_exist)
{
    sb_assert(nullptr != internal::g_vfs);
    sb_assert(nullptr != path);
    sb_warn(isVFSPathValid(path));

    FileHdl file_hdl = internal::g_vfs->openFileWrite(path, mode, fmt);

    if (create_if_not_exist && !isValid(file_hdl))
    {
        file_hdl = createFileWrite(path, fmt);
    }

    return file_hdl;
}

sb::FileHdl sb::virtual_file_system::openFileRead(char const * path, FileFormat fmt)
{
    sb_assert(nullptr != internal::g_vfs);
    sb_assert(nullptr != path);
    sb_warn(isVFSPathValid(path));

    return internal::g_vfs->openFileRead(path, fmt);
}

void sb::virtual_file_system::closeFile(FileHdl hdl)
{
    sb_assert(nullptr != internal::g_vfs);
    sb_assert(isValid(hdl));

    return internal::g_vfs->closeFile(hdl);
}

sb::FileHdl sb::virtual_file_system::createFileWrite(char const * path, FileFormat fmt)
{
    sb_assert(nullptr != internal::g_vfs);
    sb_assert(nullptr != path) sb_warn(isVFSPathValid(path));

    return internal::g_vfs->createFileWrite(path, fmt);
}

sb::FileHdl sb::virtual_file_system::createFile(char const * path, FileFormat fmt)
{
    sb_assert(nullptr != internal::g_vfs);
    sb_assert(nullptr != path) sb_warn(isVFSPathValid(path));

    return internal::g_vfs->createFileReadWrite(path, fmt);
}

sb::FileSize sb::virtual_file_system::readFile(FileHdl hdl, sbstd::span<u8> buffer, FileSize cnt)
{
    sb_assert(nullptr != internal::g_vfs);
    sb_assert(-1 <= cnt);
    sb_assert(integral_cast<FileSize>(buffer.size()) >= cnt);

    if (sb_expect(isValid(hdl)))
    {
        return internal::g_vfs->readFile(hdl, buffer.data(), (cnt == -1) ? (FileSize)buffer.size() : cnt);
    }

    return 0;
}

sb::FileSize sb::virtual_file_system::writeFile(FileHdl hdl, sbstd::span<u8 const> buffer, FileSize cnt)
{
    sb_assert(nullptr != internal::g_vfs);
    sb_assert(-1 <= cnt);
    sb_assert(integral_cast<FileSize>(buffer.size()) >= cnt);

    if (sb_expect(isValid(hdl)))
    {
        return internal::g_vfs->writeFile(hdl, buffer.data(), (cnt == -1) ? (FileSize)buffer.size() : cnt);
    }

    return 0;
}

sb::FileSize sb::virtual_file_system::getFileLength(FileHdl hdl)
{
    sb_assert(nullptr != internal::g_vfs);

    if (sb_expect(isValid(hdl)))
    {
        return internal::g_vfs->getFileLength(hdl);
    }

    return 0;
}

sb::FileProps sb::virtual_file_system::getFileProps(FileHdl hdl)
{
    sb_assert(nullptr != internal::g_vfs);

    if (sb_expect(isValid(hdl)))
    {
        return internal::g_vfs->getFileProps(hdl);
    }

    return {};
}

sb::b8 sb::virtual_file_system::fileExists(char const * path)
{
    sb_assert(nullptr != internal::g_vfs);

    return internal::g_vfs->fileExists(path);
}

sbstd::span<sb::u8> sb::virtual_file_system::readFile(char const * path, IAllocator & alloc, FileFormat fmt)
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
