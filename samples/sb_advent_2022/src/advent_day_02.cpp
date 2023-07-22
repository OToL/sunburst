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

enum HandSign : usize
{
    ROCK,
    PAPER,
    SCISSOR
};

enum MatchResult : usize
{
    LOOSE,
    DRAW,
    WIN
};

[[maybe_unused]] constexpr usize HandSignScore[3] = {1, 2, 3};

[[maybe_unused]] constexpr HandSign HandMatchWin[3] = {SCISSOR, ROCK, PAPER};
[[maybe_unused]] constexpr HandSign HandMatchLoose[3] = {PAPER, SCISSOR, ROCK};

usize getWrongScore(HandSign mySign, HandSign otherSign)
{
    usize score = 0;

    if (mySign == otherSign)
    {
        score = 3;
    }
    else if (HandMatchWin[mySign] == otherSign)
    {
        score = 6;
    }
    else
    {
        score = 0;
    }

    score += HandSignScore[mySign];

    return score;
}

usize getCorrectScore(MatchResult result, HandSign otherSign)
{
    usize score = 0;
    HandSign my_sign = otherSign;

    if (result == DRAW)
    {
        score = 3;
    }
    else if (result == WIN)
    {
        my_sign = HandMatchLoose[otherSign];
        score = 6;
    }
    else
    {
        my_sign = HandMatchWin[otherSign];
        score = 0;
    }

    score += HandSignScore[my_sign];

    return score;
}

// @todo: implement string formatting with 'char' parameter
// @todo: implement string formatting with enum parameter

// Result
//   9177
//   12111
void advent::runDay02()
{
    sb_log_info("Running advent day 02 trivia ...\n");

    FileHdl file_hdl = vfs::openFileRead("/data/day02.txt");
    sb_assert(file_hdl.isValid());

    FileSize const file_len = vfs::getFileLength(file_hdl);
    MemoryArena const file_buffer = sb_malloc(GHEAP, file_len + 1);
    u8 * const byte_buffer = static_cast<u8 *>(file_buffer.data);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;

    slw::string_view file_string_buffer(reinterpret_cast<char const *>(byte_buffer));

    auto const tokens = splitString(file_string_buffer, '\n');
    sb_log_info("token count = {}", tokens.size());

    usize const total_wrong_score = std::accumulate(begin(tokens), end(tokens), 0, [](usize curr_value, slw::string_view token) -> usize {

        slw::string_view entry = trim(token);
        if (entry.empty())
        {
            return curr_value;
        }

        auto const mySign = static_cast<HandSign>(entry[2] - 'X');
        auto const otherSign = static_cast<HandSign>(entry[0] - 'A');

        return curr_value + getWrongScore(mySign, otherSign);
    });

    usize const total_good_score = std::accumulate(begin(tokens), end(tokens), 0, [](usize curr_value, slw::string_view token) -> usize {

        slw::string_view entry = trim(token);
        if (entry.empty())
        {
            return curr_value;
        }

        auto const match_res = static_cast<MatchResult>(entry[2] - 'X');
        auto const opponent_sign = static_cast<HandSign>(entry[0] - 'A');

        return curr_value += getCorrectScore(match_res, opponent_sign);
    });

    sb_log_info("Total wrong score = {}", total_wrong_score);
    sb_log_info("Total good score = {}", total_good_score);

    sb_free(GHEAP, byte_buffer);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;
    vfs::closeFile(file_hdl);
}
