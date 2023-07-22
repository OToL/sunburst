#include "advent.h"
#include "utility.h"

#include <sb_core/log.h>
#include <sb_core/error.h>
#include <sb_core/io/virtual_file_system.h>
#include <sb_core/io/file_hdl.h>
#include <sb_core/memory/memory_arena.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/containers/dynamic_fix_array.h>
#include <sb_core/string/string.h>

#include <sb_slw/string_view>

#include <ranges>
#include <charconv>
#include <algorithm>
#include <xutility>
#include <functional>

using namespace sb;

struct FSNode;

struct FSFile
{
    FSNode * parent = nullptr;
    String name;
    usize size = 0;
};

struct FSNode
{
    FSNode * parent = nullptr;
    String name;
    usize size = 0;
    DArray<FSNode *> dirs;
    DArray<FSFile *> files;
};

void visitFileSystem(FSNode * node, std::function<void(FSNode *)> const & fun, b8 pre_call = true)
{
    if (pre_call)
    {
        fun(node);
    }

    for (auto child_dir : node->dirs)
    {
        visitFileSystem(child_dir, fun, pre_call);
    }

    if (!pre_call)
    {
        fun(node);
    }
}

// Solution
//   1315285
//   9847279
// @todo: use std::visit
// @todo: do not use inner loop for 'ls' command
// @todo: use allocator to avoid node memory leak
// @todo use inheritance for the different types of nodes
// @todo: use string_view instead of String for node name
void advent::runDay07()
{
    sb_log_info("Running advent day 07 trivia ...\n");

    FileHdl file_hdl = vfs::openFileRead("/data/day07.txt");
    sb_assert(file_hdl.isValid());

    FileSize const file_len = vfs::getFileLength(file_hdl);
    MemoryArena const file_buffer = sb_malloc(GHEAP, file_len + 1);
    u8 * const byte_buffer = static_cast<u8 *>(file_buffer.data);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;

    slw::string_view file_string_buffer(reinterpret_cast<char const *>(byte_buffer));

    FSNode root_node;
    FSNode * curr_node = &root_node;

    auto const tokens = splitString(file_string_buffer, '\n');
    sb_assert(trim(tokens[0]) == "$ cd /");
    root_node.name = "/";

    for (usize line_idx = 1; line_idx < tokens.size(); ++line_idx)
    {
        auto token = trim(tokens[line_idx]);

        if (token.empty())
        {
            continue;
        }

        auto sub_tokens = splitString(token, ' ');
        if (sub_tokens[0] == "$")
        {
            slw::string_view const cmd_str = sub_tokens[1];
            sb_assert((cmd_str == "cd") || (cmd_str == "ls"));

            if (cmd_str == "cd")
            {
                slw::string_view cmd_arg = trim(sub_tokens[2]);
                if (cmd_arg == "..")
                {
                    if (curr_node->parent != nullptr)
                    {
                        curr_node = curr_node->parent;
                    }
                    else
                    {
                        sb_report_error("you are going too high ...");
                    }
                }
                else
                {
                    auto const iter =
                        slw::find_if(begin(curr_node->dirs), end(curr_node->dirs), [cmd_arg](FSNode const * node) { return node->name == cmd_arg; });
                    sb_assert(iter != end(curr_node->dirs));

                    curr_node = *iter;
                }
            }
            else if (cmd_str == "ls")
            {
                continue;
            }
            else
            {
                sb_report_error("invalid command");
            }
        }
        else
        {
            if (sub_tokens[0] == "dir")
            {
                auto const iter = slw::find_if(begin(curr_node->dirs), end(curr_node->dirs),
                                               [name = sub_tokens[1]](FSNode const * node) { return node->name == name; });

                if (iter == end(curr_node->dirs))
                {
                    auto * new_node = sb_new(GHEAP, FSNode);
                    new_node->name = sub_tokens[1];
                    new_node->parent = curr_node;
                    curr_node->dirs.push_back(new_node);
                }
                else
                {
                    sb_report_error("wow");
                }
            }
            else
            {
                auto const iter = slw::find_if(begin(curr_node->files), end(curr_node->files),
                                               [name = sub_tokens[1]](FSFile const * node) { return node->name == name; });

                if (iter == end(curr_node->files))
                {
                    auto * new_node = sb_new(GHEAP, FSFile);
                    new_node->name = sub_tokens[1];
                    auto res = std::from_chars(sub_tokens[0].data(), sub_tokens[0].data() + sub_tokens[0].size(), new_node->size);
                    sb_assert(res.ec == std::errc());
                    new_node->parent = curr_node;

                    curr_node->files.push_back(new_node);
                }
                else
                {
                    sb_report_error("wow");
                }
            }
        }
    }

    visitFileSystem(
        &root_node,
        [](FSNode * node) {
            for (auto * file : node->files)
            {
                node->size += file->size;
            }

            for (auto * dir : node->dirs)
            {
                node->size += dir->size;
            }
        },
        false);

    visitFileSystem(&root_node, [](FSNode * node) {
        usize depth = 0;
        FSNode * parent_iter = node;
        while (parent_iter->parent != nullptr)
        {
            ++depth;
            parent_iter = parent_iter->parent;
            printf("-");
        }

        printf("%s %d\n", node->name.c_str(), (int)node->size);

        if (!node->files.empty())
        {
            for (auto file : node->files)
            {
                for (usize i = 0; i != (depth + 1); ++i)
                {
                    printf("-");
                }
                printf("%s %d\n", file->name.c_str(), (int)file->size);
            }
        }
    });

    usize total_size = 0;
    visitFileSystem(&root_node, [&total_size](FSNode * node) {
        if (node->size <= 100000)
        {
            total_size += node->size;
        }
    });

    usize size_to_free = 30000000 - (70000000 - root_node.size);
    usize smallest_size = std::numeric_limits<usize>::max();
    visitFileSystem(
        &root_node,
        [size_to_free, &smallest_size](FSNode * node) {
        if (node->size >= size_to_free)
            {
                smallest_size = slw::min(node->size, smallest_size);
            }
        },
        false);

    sb_log_info("Total #1 {}", total_size);
    sb_log_info("Total #2 {}", smallest_size);

    sb_free(GHEAP, byte_buffer);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;
    vfs::closeFile(file_hdl);
}
