#include "advent.h"
#include "utility.h"

#include <sb_core/log.h>
#include <sb_core/error.h>
#include <sb_core/io/virtual_file_system.h>
#include <sb_core/io/file_hdl.h>
#include <sb_core/memory/memory_arena.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/global_heap.h>

#include <sb_slw/string_view>
#include <numeric>

using namespace sb;

// Solution
//   7737
//   2697
void advent::runDay03()
{
    sb_log_info("Running advent day 03 trivia ...\n");

    FileHdl file_hdl = vfs::openFileRead("/data/day03.txt");
    sb_assert(file_hdl.isValid());

    FileSize const file_len = vfs::getFileLength(file_hdl);
    MemoryArena const file_buffer = sb_malloc(GHEAP, file_len + 1);
    u8 * const byte_buffer = static_cast<u8 *>(file_buffer.data);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;

    slw::string_view file_string_buffer(reinterpret_cast<char const *>(byte_buffer));

    auto const tokens = splitString(file_string_buffer, '\n');
    sb_log_info("token count = {}", tokens.size());

    usize total_score = 0;
    for (auto const & token : tokens)
    {
        slw::string_view const trim_token = trim(token);
        usize const comp_item_cnt = trim_token.size() / 2;

        slw::string_view const rcomp = trim_token.substr(comp_item_cnt);

        for (char const item : trim_token.substr(0, comp_item_cnt))
        {
            if (slw::string_view::npos != rcomp.find_first_of(item))
            {
                if (('a' <= item) && ('z' >= item))
                {
                    total_score += item - 'a' + 1;
                }
                else
                {
                    total_score += item - 'A' + 27;
                }

                break;
            }
        }
    }

    usize total_dup_score = 0;
    u32 item_cnt[52] = {};
    u32 elf_item_cnt[52] = {};

    for (usize line_idx = 0; (line_idx + 3) < tokens.size(); line_idx += 3)
    {
        zeroMemory(item_cnt);
        usize dup_item = 0;

        for (usize idx = 0; idx < 3; ++idx)
        {
            slw::string_view const trim_token = trim(tokens[line_idx + idx]);
            zeroMemory(elf_item_cnt);

            for (char const item : trim_token)
            {
                usize array_idx = 0;
                if (('a' <= item) && ('z' >= item))
                {
                    array_idx = item - 'a';
                }
                else
                {
                    array_idx = item - 'A' + 26;
                }

                elf_item_cnt[array_idx] = 1;
            }

            // @todo: use size() from slw
            for (usize item_idx = 0; item_idx < std::size(item_cnt); ++item_idx)
            {
                item_cnt[item_idx] += elf_item_cnt[item_idx];

                if (3 == item_cnt[item_idx])
                {
                    sb_assert(dup_item == 0);
                    total_dup_score += (item_idx + 1);
                    dup_item = item_idx + 1;
                }
            }
        }

        sb_assert(0 != dup_item, "did not find badge");
    }

    sb_log_info("Total score = {}", total_score);
    sb_log_info("Total badge score = {}", total_dup_score);

    sb_free(GHEAP, byte_buffer);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;
    vfs::closeFile(file_hdl);
}
