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

// Solution
//   1832
//   157320
// @todo: use iterators
// @todo: use less code
void advent::runDay08()
{
    sb_log_info("Running advent day 08 trivia ...\n");

    FileHdl file_hdl = vfs::openFileRead("/data/day08.txt");
    sb_assert(file_hdl.isValid());

    FileSize const file_len = vfs::getFileLength(file_hdl);
    MemoryArena const file_buffer = sb_malloc(GHEAP, file_len + 1);
    u8 * const byte_buffer = static_cast<u8 *>(file_buffer.data);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;

    slw::string_view file_string_buffer(reinterpret_cast<char const *>(byte_buffer));
    auto tokens = splitString(file_string_buffer, '\n');

    for (auto &token : tokens)
    {
        token = trim(token);
    }

    if (tokens.back().empty())
    {
        tokens.resize(tokens.size() - 1);
    }

    usize const row_count = tokens.size();
    usize const column_count = tokens[0].size();
    usize visible_tree_cnt = column_count * 2 + (row_count - 2) * 2;

    for (usize row_idx = 1; row_idx < (row_count - 1); ++row_idx)
    {
        auto const & curr_row = tokens[row_idx];

        for (usize column_idx = 1; column_idx < (column_count - 1); ++column_idx)
        {
            auto const & curr_value = curr_row[column_idx];

            auto const curr_tree_iter = begin(curr_row) + column_idx;
            auto hiding_tree = slw::find_if(begin(curr_row), curr_tree_iter, [curr_value](char const tree_val) { return tree_val >= curr_value; });

            if (hiding_tree == curr_tree_iter)
            {
                ++visible_tree_cnt;
                continue;
            }

            hiding_tree = slw::find_if(curr_tree_iter + 1, end(curr_row), [curr_value](char const tree_val) { return tree_val >= curr_value; });
            if (hiding_tree == end(curr_row))
            {
                ++visible_tree_cnt;
                continue;
            }

            usize vis_row_idx = 0;
            for (; vis_row_idx < row_idx; ++vis_row_idx)
            {
                if (tokens[vis_row_idx][column_idx] >= curr_value)
                {
                    break;
                }
            }

            if (vis_row_idx == row_idx)
            {
                ++visible_tree_cnt;
                continue;
            }

            vis_row_idx = row_idx + 1;
            for (; vis_row_idx < row_count; ++vis_row_idx)
            {
                if (tokens[vis_row_idx][    column_idx] >= curr_value)
                {
                    break;
                }
            }

            if (vis_row_idx == row_count)
            {
                ++visible_tree_cnt;
                continue;
            }
        }
    }

    usize higher_scenic_value = 0;
    for (usize row_idx = 1; row_idx < (row_count - 1); ++row_idx)
    {
        auto const & curr_row = tokens[row_idx];

        for (usize column_idx = 1; column_idx < (column_count - 1); ++column_idx)
        {
            auto const curr_value = curr_row[column_idx];

            u32 lscore = 0;
            for (i32 col_idx = (i32)(column_idx - 1) ; col_idx >= 0 ; --col_idx)
            {
                ++lscore;
                if (curr_value <= curr_row[col_idx])
                {
                    break;
                }
            }

            u32 rscore = 0;
            for (i32 col_idx = (i32)(column_idx + 1) ; col_idx < (i32)column_count ; ++col_idx)
            {
                ++rscore;
                if (curr_value <= curr_row[col_idx])
                {
                    break;
                }
            }


            u32 tscore = 0;
            for (i32 rw_idx = (i32)(row_idx - 1) ; rw_idx >= 0 ; --rw_idx)
            {
                ++tscore;
                if (curr_value <= tokens[rw_idx][column_idx])
                {
                    break;
                }
            }

            u32 bscore = 0;
            for (i32 rw_idx = (i32)(row_idx + 1) ; rw_idx < (i32)row_count ; ++rw_idx)
            {
                ++bscore;
                if (curr_value <= tokens[rw_idx][column_idx])
                {
                    break;
                }
            }

            usize total_score = lscore * rscore * tscore * bscore;
            higher_scenic_value = std::max(total_score, higher_scenic_value);
        }
    }

     sb_log_info("Visible tree count {}", visible_tree_cnt);
     sb_log_info("Higher scenic score {}", higher_scenic_value);

    sb_free(GHEAP, byte_buffer);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;
    vfs::closeFile(file_hdl);
}
