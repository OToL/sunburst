#include "advent.h"
#include "utility.h"

#include <sb_core/core.h>
#include <sb_core/system.h>
#include <sb_core/log.h>
#include <sb_core/error.h>
#include <sb_core/io/virtual_file_system.h>
#include <sb_core/io/io.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/string/string.h>
#include <sb_core/containers/dynamic_array.h>

#include <sb_slw/span>
#include <sb_slw/algorithm>
#include <sb_slw/string_view>

#include <charconv>

using namespace sb;

// @todo: move string trim sb_core/string/utility.h
// @todo: move string split sb_core/string/utility.h
// @todo: implement readline working on a buffer/string_view
// @todo: try incremental insertion to identify top 3 i.e. use lower_bound
// @todo: try using simd to tokenize

// Solution:
//    - Max supply value: 67633
//    - Sum of top 3 supply values: 199628
void advent::runDay01()
{
    sb_log_info("Running advent day 01 trivia ...\n");

    FileHdl file_hdl = vfs::openFileRead("/data/day01.txt");
    sb_assert(file_hdl.isValid());

    auto const file_len = vfs::getFileLength(file_hdl);

    MemoryArena const file_buffer = sb_malloc(GHEAP, file_len + 1);
    u8 * const byte_buffer = static_cast<u8 *>(file_buffer.data);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;

    slw::string_view file_string_buffer(reinterpret_cast<char const *>(byte_buffer));

    auto const tokens = splitString(file_string_buffer, '\n');

    DArray<usize> allSupplies;
    allSupplies.reserve(tokens.size());

    usize max_supply_value = 0;
    usize curr_supply_value = 0;
    for (auto const & token : tokens)
    {
        auto const trim_token = trim(token);
        if (trim_token.empty())
        {
            if (curr_supply_value > max_supply_value)
            {
                max_supply_value = curr_supply_value;
            }

            allSupplies.push_back(curr_supply_value);
            curr_supply_value = 0;
        }
        else
        {
            usize value = 0;
            auto token_res = std::from_chars(trim_token.data(), trim_token.data() + trim_token.size(), value);
            sb_assert(token_res.ec == std::errc{}, "error {}", trim_token.data());
            sb_assert(value != 0);

            curr_supply_value += value;
        }
    }

    sb_free(GHEAP, file_buffer);

    slw::sort(begin(allSupplies), end(allSupplies), std::greater<usize>{});

    sb_log_info("max value = {}", max_supply_value);
    sb_log_info("max 3 values = {}", allSupplies[0] + allSupplies[1] + allSupplies[2]);

    vfs::closeFile(file_hdl);
}

