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

#include <sb_slw/string_view>

#include <ranges>
#include <algorithm>

using namespace sb;

template <usize CAPACITY>
b8 addToCharWindow(DFArray<char, CAPACITY> & char_window, char const val)
{
    if (char_window.full())
    {
        std::ranges::rotate(char_window.begin(), char_window.begin() + 1, char_window.end());
        char_window.back() = val;

        usize curr_mask = 0ULL;
        for (auto const win_val : char_window)
        {
            usize const bit_offset = win_val - 'a';
            sb_assert(bit_offset < 64);

            usize const bit_val = 1 << bit_offset;
            if ((bit_val & curr_mask) != 0)
            {
                return false;
            }
            else
            {
                curr_mask |= bit_val;
            }
        }

        return true;
    }
    else
    {
        char_window.push_back(val);
    }

    return false;
}

// Solution
//   1287
//   3716
void advent::runDay06()
{
    sb_log_info("Running advent day 06 trivia ...\n");

    FileHdl file_hdl = vfs::openFileRead("/data/day06.txt");
    sb_assert(file_hdl.isValid());

    FileSize const file_len = vfs::getFileLength(file_hdl);
    MemoryArena const file_buffer = sb_malloc(GHEAP, file_len + 1);
    u8 * const byte_buffer = static_cast<u8 *>(file_buffer.data);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;

    slw::string_view file_string_buffer(reinterpret_cast<char const *>(byte_buffer));

    DFArray<char, 4> packet_window;
    DFArray<char, 14> msg_window;

    auto const packet_it =
        std::ranges::find_if(file_string_buffer, [&char_window = packet_window](char const val) { return addToCharWindow(char_window, val); });
    auto const msg_it =
        std::ranges::find_if(file_string_buffer, [&char_window = msg_window](char const val) { return addToCharWindow(char_window, val); });

    sb_assert(packet_it != end(file_string_buffer));
    sb_assert(msg_it != end(file_string_buffer));

    sb_log_info("temp result {}", std::distance(file_string_buffer.begin(), packet_it) + 1);
    sb_log_info("temp result {}", std::distance(file_string_buffer.begin(), msg_it) + 1);

    sb_free(GHEAP, byte_buffer);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;
    vfs::closeFile(file_hdl);
}

