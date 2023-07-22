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
#include <vector>
#include <xutility>
#include <functional>

using namespace sb;

struct Vect2D
{
    i32 x = 0;
    i32 y = 0;
};

Vect2D operator+(Vect2D lv, Vect2D rv)
{
    return {lv.x + rv.x, lv.y + rv.y};
}

Vect2D operator-(Vect2D lv, Vect2D rv)
{
    return {lv.x - rv.x, lv.y - rv.y};
}

Vect2D getMoveDirection(char direction)
{
    switch (direction)
    {
        case 'R':
            return {1, 0};
        case 'U':
            return {0, -1};
        case 'L':
            return {-1, 0};
        case 'D':
            return {0, 1};
    }

    sb_report_error("unknown direction");

    return {};
}

Vect2D correctTailPosition(Vect2D head, Vect2D tail, char direction)
{
    Vect2D const dist = head - tail;
    if ((std::abs(dist.x) <= 1) && (std::abs(dist.y) <= 1))
    {
        return tail;
    }

    return head - getMoveDirection(direction);
}

Vect2D correctTailPosition2(Vect2D head, Vect2D prv_head, Vect2D tail, [[maybe_unused]]char direction)
{
    Vect2D const dist = head - tail;
    if ((std::abs(dist.x) <= 1) && (std::abs(dist.y) <= 1))
    {
        return tail;
    }

    return prv_head;
}

Vect2D correctTailPosition3(Vect2D head, Vect2D tail, [[maybe_unused]]char direction)
{
    Vect2D const dist = head - tail;
    if ((std::abs(dist.x) <= 1) && (std::abs(dist.y) <= 1))
    {
        return tail;
    }

    Vect2D move_dir = 
    {
        std::min(1, std::abs(dist.x)),
        std::min(1, std::abs(dist.y))
    };

    move_dir.x = (dist.x < 0)?-1 * move_dir.x:move_dir.x; 
    move_dir.y = (dist.y < 0)?-1 * move_dir.y:move_dir.y; 

    return tail + move_dir;
}

// Solution
//   6011
//   2419
void advent::runDay09()
{
    sb_log_info("Running advent day 09 trivia ...\n");

    FileHdl file_hdl = vfs::openFileRead("/data/day09.txt");
    sb_assert(file_hdl.isValid());

    FileSize const file_len = vfs::getFileLength(file_hdl);
    MemoryArena const file_buffer = sb_malloc(GHEAP, file_len + 1);
    u8 * const byte_buffer = static_cast<u8 *>(file_buffer.data);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;

    slw::string_view file_string_buffer(reinterpret_cast<char const *>(byte_buffer));
    auto tokens = splitString(file_string_buffer, '\n');

    for (auto & token : tokens)
    {
        token = trim(token);
    }

    if (tokens.back().empty())
    {
        tokens.resize(tokens.size() - 1);
    }

    i32 constexpr BOARD_BOUND = 1000;
    b8 board[BOARD_BOUND][BOARD_BOUND] = {};

    Vect2D head = {BOARD_BOUND / 2, BOARD_BOUND / 2};
    Vect2D tail = head;

    usize visited_cell_cnt = 0;
    for (auto const & token : tokens)
    {
        auto const direction = token[0];
        i32 move_count = 0;
        std::from_chars(token.data() + 2, token.data() + token.size(), move_count);
        auto const move_dir = getMoveDirection(direction);

        for (i32 move_idx = 0; move_idx != move_count; ++move_idx)
        {
            auto new_head = head + move_dir;

            sb_assert(new_head.y >= 0);
            sb_assert(new_head.y < BOARD_BOUND);
            sb_assert(new_head.x >= 0);
            sb_assert(new_head.x < BOARD_BOUND);

            head = new_head;
            tail = correctTailPosition(head, tail, direction);

            if (!board[tail.x][tail.y])
            {
                ++visited_cell_cnt;
                board[tail.x][tail.y] = true;
            }
        }
    }

    usize visited_cell_long_cnt = 0;
    Vect2D chain[10] = {};
    head = {BOARD_BOUND / 2, BOARD_BOUND / 2};
    tail = head;
    memset(board, 0, BOARD_BOUND * BOARD_BOUND);

    std::fill(std::begin(chain), std::end(chain), Vect2D{BOARD_BOUND / 2, BOARD_BOUND / 2});

    for (auto const & token : tokens)
    {
        auto const direction = token[0];
        i32 move_count = 0;
        std::from_chars(token.data() + 2, token.data() + token.size(), move_count);
        auto const move_dir = getMoveDirection(direction);

        for (i32 move_idx = 0; move_idx != move_count; ++move_idx)
        {
            auto pre_pos = chain[0];
            chain[0] = chain[0] + move_dir;

            //sb_assert(chain[0].y >= 0);
            //sb_assert(chain[0].y < BOARD_BOUND);
            //sb_assert(chain[0].x >= 0);
            //sb_assert(chain[0].x < BOARD_BOUND);

            for (usize chain_idx = 1; chain_idx != std::size(chain); ++chain_idx)
            {
                auto prev_pos2 = chain[chain_idx];
                chain[chain_idx] = correctTailPosition3(chain[chain_idx - 1], chain[chain_idx], direction);
                pre_pos = prev_pos2;
            }

            tail = chain[std::size(chain) - 1];

            if (!board[tail.x][tail.y])
            {
                ++visited_cell_long_cnt;
                board[tail.x][tail.y] = true;
            }
        }
    }

    sb_log_info("Visited cell count {}", visited_cell_cnt);
    sb_log_info("Visited call count long {}", visited_cell_long_cnt);

    sb_free(GHEAP, byte_buffer);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;
    vfs::closeFile(file_hdl);
}
