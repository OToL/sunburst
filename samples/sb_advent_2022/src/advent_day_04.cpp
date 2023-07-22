#include "advent.h"
#include "utility.h"

#include <sb_core/log.h>
#include <sb_core/error.h>
#include <sb_core/io/virtual_file_system.h>
#include <sb_core/io/file_hdl.h>
#include <sb_core/memory/memory_arena.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/containers/dynamic_array.h>

#include <sb_slw/string_view>
#include <numeric>
#include <charconv>

using namespace sb;

struct TeamSpan
{
    u32 begin = 0;
    u32 end = 0;
};

b8 areFullyContained(TeamSpan lteam, TeamSpan rteam)
{
    return ((lteam.begin >= rteam.begin) && (lteam.end <= rteam.end)) || ((rteam.begin >= lteam.begin) && (rteam.end <= lteam.end));
}

TeamSpan getTeamSpan(slw::string_view team_str)
{
    auto const team_idx = team_str.find('-');
    sb_assert(slw::string_view::npos != team_idx);

    TeamSpan team_desc;
    std::from_chars(team_str.data(), team_str.data() + team_idx, team_desc.begin);
    std::from_chars(team_str.data() + team_idx + 1, team_str.data() + team_str.size(), team_desc.end);

    return team_desc;
}

// Solution
//   530
//   903
void advent::runDay04()
{
    sb_log_info("Running advent day 04 trivia ...\n");

    FileHdl file_hdl = vfs::openFileRead("/data/day04.txt");
    sb_assert(file_hdl.isValid());

    FileSize const file_len = vfs::getFileLength(file_hdl);
    MemoryArena const file_buffer = sb_malloc(GHEAP, file_len + 1);
    u8 * const byte_buffer = static_cast<u8 *>(file_buffer.data);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;

    slw::string_view file_string_buffer(reinterpret_cast<char const *>(byte_buffer));

    auto const tokens = splitString(file_string_buffer, '\n');
    sb_log_info("token count = {}", tokens.size());

    usize total_fully_contained_score = 0;
    for (auto const & token : tokens)
    {
        if (token.empty())
        {
            continue;
        }

        // 48-50,48-49
        slw::string_view const trim_token = trim(token);
        sb::DArray<slw::string_view> const team = splitString(trim_token, ',');

        TeamSpan const lteam = getTeamSpan(team[0]);
        TeamSpan const rteam = getTeamSpan(team[1]);

        if (areFullyContained(lteam, rteam))
        {
            //sb_log_info("Overalpping ");
            ++total_fully_contained_score;
        }
    }

    usize total_overlap_score = 0;
    for (auto const & token : tokens)
    {
        if (token.empty())
        {
            continue;
        }

        // 48-50,48-49
        slw::string_view const trim_token = trim(token);
        sb::DArray<slw::string_view> const team = splitString(trim_token, ',');

        TeamSpan const lteam = getTeamSpan(team[0]);
        TeamSpan const rteam = getTeamSpan(team[1]);

        TeamSpan const super_span =
        {
            std::max(lteam.begin, rteam.begin),
            std::min(rteam.end, lteam.end)
        };

        if (super_span.end >= super_span.begin)
        {
            ++total_overlap_score;
        }
    }

    sb_log_info("Total score = {}", total_fully_contained_score);
    sb_log_info("Total score overlap = {}", total_overlap_score);

    sb_free(GHEAP, byte_buffer);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0; vfs::closeFile(file_hdl); }
