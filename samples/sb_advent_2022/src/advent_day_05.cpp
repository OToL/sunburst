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
#include <sb_core/string/fix_string.h>

#include <sb_slw/string_view>
#include <numeric>
#include <charconv>
#include <ranges>

using namespace sb;

enum class ParseState
{
    INITIAL_STATE,
    OPERATIONS
};

// @todo: support slw::string_view for string format / sb_log
// @todo: support 'char' parameter for string format / sb_log
// @todo: rework the code globally
// @todo: try to use regex
// @todo: use differ container than DArray
// Solution
//   TDCHVHJTG
//   NGCMPJLHV
void advent::runDay05()
{
    sb_log_info("Running advent day 05 trivia ...\n");

    FileHdl file_hdl = vfs::openFileRead("/data/day05.txt");
    sb_assert(file_hdl.isValid());

    FileSize const file_len = vfs::getFileLength(file_hdl);
    MemoryArena const file_buffer = sb_malloc(GHEAP, file_len + 1);
    u8 * const byte_buffer = static_cast<u8 *>(file_buffer.data);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;

    slw::string_view file_string_buffer(reinterpret_cast<char const *>(byte_buffer));

    auto const tokens = splitString(file_string_buffer, '\n');
    sb_log_info("token count = {}", tokens.size());

    for (usize test_idx : std::ranges::iota_view{0, 2})
    {
        DArray<char> crates[9];
        ParseState parse_state = ParseState::INITIAL_STATE;

        for (auto const & token : tokens)
        {
            auto trim_token = trim(token);
            if (trim_token.empty())
            {
                if (ParseState::INITIAL_STATE == parse_state)
                {
                    for (auto & crate : crates)
                    {
                        std::ranges::reverse(crate);
                    }

                    parse_state = ParseState::OPERATIONS;
                }
                else {
                    break;
                }

                continue;
            }

            if (ParseState::INITIAL_STATE == parse_state)
            {
                auto item_id_offset = trim_token.find('[');

                while (slw::string_view::npos != item_id_offset)
                {
                    auto const item_id_end = trim_token.find(']', item_id_offset);
                    sb_assert(slw::string_view::npos != item_id_end);

                    usize const crate_id = item_id_offset >> 2;
                    sb_assert(crate_id < std::size(crates));

                    crates[crate_id].push_back(trim_token[item_id_offset + 1]);

                    item_id_offset = trim_token.find('[', item_id_end);
                }
            }
            else
            {
                sb_assert(trim_token.starts_with("move "));

                auto const item_cnt_offset = strlen("move ");
                auto const src_crate_offset = trim_token.find("from ") + strlen("from ");
                auto const dst_crate_offset = trim_token.find("to ") + strlen("to ");

                char const * const token_end = trim_token.data() + trim_token.size();

                usize nb_items = 0;
                usize src_crate_id = 0;
                usize dst_crate_id = 0;

                auto res = std::from_chars(trim_token.data() + item_cnt_offset, token_end, nb_items);
                sb_assert(res.ec == std::errc());
                res = std::from_chars(trim_token.data() + src_crate_offset, token_end, src_crate_id);
                sb_assert(res.ec == std::errc());
                res = std::from_chars(trim_token.data() + dst_crate_offset, token_end, dst_crate_id);
                sb_assert(res.ec == std::errc());

                sb_assert(src_crate_id > 0);
                sb_assert(dst_crate_id > 0);

                auto & dst_crate = crates[dst_crate_id - 1];
                auto & src_crate = crates[src_crate_id - 1];
                auto const actual_move_items = slw::min(nb_items, src_crate.size());

                dst_crate.resize(dst_crate.size() + actual_move_items);
                if (test_idx == 0)
                {
                    slw::copy(src_crate.rbegin(), src_crate.rend() - (src_crate.size() - actual_move_items), dst_crate.end() - actual_move_items);
                }
                else
                {
                    slw::copy(src_crate.end() - actual_move_items, src_crate.end(), dst_crate.end() - actual_move_items);
                }

                src_crate.resize(src_crate.size() - actual_move_items);
            }
        }

        FixString<10> top_items;
        for (auto const & crate : crates)
        {
            if (!crate.empty())
            {
                top_items.push_back(crate[crate.size() - 1]);
            }
        }

        sb_log_info("Top items #{} = {}", test_idx, top_items.c_str());
    }

    sb_free(GHEAP, byte_buffer);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;
    vfs::closeFile(file_hdl);
}
