#include "local_file_system.h"
#include <sb_core/io/virtual_file_system.h>
#include <sb_core/io/io.h>
#include <sb_core/io/path.h>
#include <sb_core/core.h>
#include <sb_core/cast.h>
#include <sb_core/log.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/memory/allocator/object_pool_allocator.h>
#include <sb_core/memory/allocator/global_heap_allocator.h>
#include <sb_core/container/dynamic_small_array.h>
#include <sb_core/container/dynamic_fix_array.h>

namespace sb {

using FixVFSPath = FixString<VFS_PATH_MAX_LEN + 1>;
using FixFSPath = FixString<LOCAL_PATH_MAX_LEN + 1>;
using FileGen = u16;

struct LayerDesc
{
    vfs::LayerName name;
    FileSystemType type;
    FixVFSPath vfs_root_path;
    FixFSPath fs_path;
};

struct FileDesc
{
    FileProps props;
    FileSystemType fs_type;
    FileGen gen;
    sb::File hdl;
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

struct VirtualFileSystemState
{
    sb_copy_protect(VirtualFileSystemState);

public:
    VirtualFileSystemState()
        : file_desc_pool(50)
    {
    }

    using LayerNameGen = u16;
    using FileDescPool = ObjectPoolAllocator<FileDesc, GlobalHeapAllocator>;

    FileDescPool file_desc_pool;
    DFArray<LayerDesc, sb::VFS_LAYER_MAX_COUNT> layers;
    DSArray<FileSystem, 5> file_systems;
    LayerNameGen curr_layer_name_gen = 0;
    FileGen curr_file_gen = 0;
    u16 opened_file_count = 0;
};

VirtualFileSystemState * g_vfs_state = nullptr;

auto findVFSLayer(VirtualFileSystemState const & vfs_data, vfs::LayerName layer_name)
{
    auto const layer_iter =
        sbstd::find_if(begin(vfs_data.layers), end(vfs_data.layers),
                       [layer_name](LayerDesc const & layer_desc) { return layer_desc.name == layer_name; });

    return layer_iter;
}

auto findVFSLayer(VirtualFileSystemState const & vfs_data, char const * layer_path)
{
    auto const layer_iter =
        sbstd::find_if(begin(vfs_data.layers), end(vfs_data.layers), [layer_path](LayerDesc const & layer_desc) {
            return strStartsWith(layer_path, layer_desc.vfs_root_path.c_str());
        });

    return layer_iter;
}

void constructFSPath(FixFSPath & fs_path, LayerDesc const & layer, char const * file_path)
{
    fs_path = layer.fs_path;
    fs_path.append(file_path + layer.vfs_root_path.length());
    normalizeLocalPath(fs_path.data());
}

FileHdl createFileDesc(VirtualFileSystemState & vfs_data, sb::File fs_file, FileProps props, FileSystemType fs_type)
{
    FileDesc * const file_desc = static_cast<FileDesc *>(vfs_data.file_desc_pool.allocate().data);
    if (sb_expect(nullptr != file_desc))
    {
        *file_desc = {.props = props, .fs_type = fs_type, .gen = ++g_vfs_state->curr_file_gen, .hdl = fs_file};

        MemoryArena arena = g_vfs_state->file_desc_pool.getArena();
        auto const base_obj = static_cast<FileDesc *>(arena.data);

        FileHelper const helper_hdl = {
            .unpacked = {integral_cast<u16>(sbstd::distance(base_obj, file_desc)), integral_cast<u16>(file_desc->gen)}};

        return FileHdl{helper_hdl.packed};
    }

    return FileHdl{};
}

FileDesc & getFileDesc(VirtualFileSystemState & vfs_data, FileHdl hdl)
{
    sb_assert(hdl.isValid());

    FileHelper const helper_hdl = {.packed = hdl.value};

    MemoryArena arena = vfs_data.file_desc_pool.getArena();
    FileDesc * const file_desc = static_cast<FileDesc *>(arena.data) + helper_hdl.unpacked.hdl;

    sb_assert(isInRange(arena, file_desc));
    sb_assert(helper_hdl.unpacked.gen == file_desc->gen);

    return *file_desc;
}

void destroyFileDesc(VirtualFileSystemState & vfs_data, FileDesc & file_desc)
{
    sb_assert(isInRange(vfs_data.file_desc_pool.getArena(), (void *)&file_desc));

    file_desc = {};
    vfs_data.file_desc_pool.deallocate(&file_desc);
}

} // namespace sb

sb::b8 sb::virtual_file_system::initialize()
{
    if (sb_expect(nullptr == g_vfs_state, "Virtual File System is already initialized"))
    {
        g_vfs_state = sb_new(GHEAP, VirtualFileSystemState);

        // order must be the same as FileSystemType enum
        g_vfs_state->file_systems.push_back(getLocalFileSystem());

        return true;
    }

    return false;
}

sb::b8 sb::virtual_file_system::terminate()
{
    if (sb_expect(nullptr != g_vfs_state, "Virtual File System is not initialized"))
    {
        sb_warn(0 == g_vfs_state->opened_file_count, "{} files are still opened", g_vfs_state->opened_file_count);

        sb_delete(GHEAP, g_vfs_state);
        g_vfs_state = nullptr;

        return true;
    }

    return false;
}

sb::FileHdl sb::virtual_file_system::openFileWrite(char const * path, FileWriteMode mode, FileFormat fmt)
{
    sb_assert(nullptr != g_vfs_state);
    sb_assert(nullptr != path);
    sb_warn(isVFSPathValid(path));

    auto const layer_iter = findVFSLayer(*g_vfs_state, path);
    if (layer_iter != end(g_vfs_state->layers))
    {
        FixFSPath fs_path;
        FileSystem & fs = g_vfs_state->file_systems[integral_cast<>(layer_iter->type)];

        constructFSPath(fs_path, *layer_iter, path);
        auto fs_file = fs.open_file_write(fs_path.c_str(), mode, fmt);

        if (fs.is_file_valid(fs_file))
        {
            FileProps const file_props = {.fmt = fmt, .access = {FileAccessFlags::WRITE}};
            FileHdl file_hdl = createFileDesc(*g_vfs_state, fs_file, file_props, layer_iter->type);

            if (sb_expect(file_hdl.isValid(), "Out of file descriptors"))
            {
                ++g_vfs_state->opened_file_count;
                return file_hdl;
            }
            else
            {
                fs.close_file(fs_file);
            }
        }
    }

    return {};
}

sb::FileHdl sb::virtual_file_system::openFileRead(char const * path, FileFormat fmt)
{
    sb_assert(nullptr != g_vfs_state);
    sb_assert(nullptr != path);
    sb_warn(isVFSPathValid(path));

    auto const layer_iter = findVFSLayer(*g_vfs_state, path);
    if (layer_iter != end(g_vfs_state->layers))
    {
        FixFSPath fs_path;
        FileSystem & fs = g_vfs_state->file_systems[integral_cast<>(layer_iter->type)];

        constructFSPath(fs_path, *layer_iter, path);
        auto fs_file = fs.open_file_read(fs_path.c_str(), fmt);

        if (fs.is_file_valid(fs_file))
        {
            FileProps const file_props = {.fmt = fmt, .access = {FileAccessFlags::READ}};
            FileHdl file_hdl = createFileDesc(*g_vfs_state, fs_file, file_props, layer_iter->type);

            if (sb_expect(file_hdl.isValid(), "Out of file descriptors"))
            {
                ++g_vfs_state->opened_file_count;
                return file_hdl;
            }
            else
            {
                fs.close_file(fs_file);
            }
        }
    }

    return {};
}

void sb::virtual_file_system::closeFile(FileHdl hdl)
{
    sb_assert(nullptr != g_vfs_state);
    sb_assert(0 < g_vfs_state->opened_file_count);

    if (sb_expect(hdl.isValid()))
    {
        auto & file_desc = getFileDesc(*g_vfs_state, hdl);

        g_vfs_state->file_systems[integral_cast<>(file_desc.fs_type)].close_file(file_desc.hdl);
        destroyFileDesc(*g_vfs_state, file_desc);

        --g_vfs_state->opened_file_count;
    }
}

sb::FileSize sb::virtual_file_system::readFile(FileHdl hdl, sbstd::span<u8> buffer, FileSize cnt)
{
    sb_assert(nullptr != g_vfs_state);
    sb_assert(-1 <= cnt);
    sb_assert(integral_cast<FileSize>(buffer.size()) >= cnt);

    if (sb_expect(hdl.isValid()))
    {
        auto const & file_desc = getFileDesc(*g_vfs_state, hdl);

        if (sb_expect(0 != (file_desc.props.access.value & FileAccessFlags::READ)))
        {
            return g_vfs_state->file_systems[integral_cast<>(file_desc.fs_type)].read_file(
                file_desc.hdl, buffer.data(), integral_cast<FileSize>(buffer.size()));
        }
    }

    return 0;
}

sb::FileSize sb::virtual_file_system::writeFile(FileHdl hdl, sbstd::span<u8 const> buffer, FileSize cnt)
{
    sb_assert(nullptr != g_vfs_state);
    sb_assert(-1 <= cnt);
    sb_assert(integral_cast<FileSize>(buffer.size()) >= cnt);

    if (sb_expect(hdl.isValid()))
    {
        auto const & file_desc = getFileDesc(*g_vfs_state, hdl);

        if (sb_expect(0 != (file_desc.props.access.value & FileAccessFlags::WRITE)))
        {
            return g_vfs_state->file_systems[integral_cast<>(file_desc.fs_type)].write_file(
                file_desc.hdl, buffer.data(), integral_cast<FileSize>(buffer.size()));
        }
    }

    return 0;
}

sb::FileSize sb::virtual_file_system::getFileLength(FileHdl hdl)
{
    sb_assert(nullptr != g_vfs_state);

    if (sb_expect(hdl.isValid()))
    {
        auto const & file_desc = getFileDesc(*g_vfs_state, hdl);
        return g_vfs_state->file_systems[integral_cast<>(file_desc.fs_type)].get_file_length(file_desc.hdl);
    }

    return 0;
}

sb::FileProps sb::virtual_file_system::getFileProps(FileHdl hdl)
{
    sb_assert(nullptr != g_vfs_state);

    if (sb_expect(hdl.isValid()))
    {
        auto const & file_desc = getFileDesc(*g_vfs_state, hdl);
        return file_desc.props;
    }

    return {};
}

sb::b8 sb::virtual_file_system::fileExists(char const * path)
{
    sb_assert(nullptr != g_vfs_state);

    auto const layer_iter = findVFSLayer(*g_vfs_state, path);

    if (layer_iter != end(g_vfs_state->layers))
    {
        FixFSPath fs_path;
        constructFSPath(fs_path, *layer_iter, path);

        return g_vfs_state->file_systems[integral_cast<>(layer_iter->type)].file_exists(fs_path.c_str());
    }

    return false;
}

sbstd::span<sb::u8> sb::virtual_file_system::readFile(char const * path, IAllocator & alloc, FileFormat fmt)
{
    auto const fd = openFileRead(path, fmt);
    if (!fd.isValid())
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

sb::vfs::MountResult sb::virtual_file_system::mountLocalFileSystem(char const * vfs_path, char const * local_path,
                                                                   LayerName name)
{
    sb_assert(nullptr != g_vfs_state);
    sb_assert(nullptr != vfs_path);
    sb_assert(nullptr != local_path);

    if (sb_dont_expect((0 == *vfs_path) || !isVFSPathValid(vfs_path), "Invalid vfs path '{}'", vfs_path))
    {
        return {};
    }

    if (sb_dont_expect((0 == *local_path) || !isLocalPathValid(local_path), "Invalid local path '{}'", local_path))
    {
        return {};
    }

    if (!isValid(name))
    {
        char buffer[125];
        auto const char_cnt = formatString(buffer, "local_filesystem_{}", g_vfs_state->curr_layer_name_gen++);
        name = hash_str::make(buffer, char_cnt);
    }

    auto const layer_iter = findVFSLayer(*g_vfs_state, name);
    if (sb_dont_expect(end(g_vfs_state->layers) != layer_iter, "Layer with this name already exists"))
    {
        return {};
    }

    auto const new_layer = g_vfs_state->layers.expand(1);
    new_layer->name = name;
    new_layer->vfs_root_path = vfs_path;
    new_layer->fs_path = local_path;
    new_layer->type = FileSystemType::LOCAL;

    normalizeLocalPath(new_layer->fs_path.data());
    if (LOCAL_PATH_SEPERATOR != new_layer->fs_path.back())
    {
        new_layer->fs_path.push_back(LOCAL_PATH_SEPERATOR);
        sb_warn(LOCAL_PATH_SEPERATOR == new_layer->fs_path.back());
    }

    if (VFS_PATH_SEPARATOR != new_layer->vfs_root_path.back())
    {
        new_layer->vfs_root_path.push_back(VFS_PATH_SEPARATOR);
        sb_warn(VFS_PATH_SEPARATOR == new_layer->vfs_root_path.back());
    }

    return {true, name};
}

sb::b8 sb::virtual_file_system::umount(LayerName name)
{
    sb_assert(nullptr != g_vfs_state);

    auto const layer_iter = findVFSLayer(*g_vfs_state, name);
    if (sb_expect(layer_iter != end(g_vfs_state->layers), "Cannot find layer to unmount"))
    {
        g_vfs_state->layers.erase(layer_iter);
        return true;
    }

    return false;
}
