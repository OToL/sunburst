#include "advent.h"
#include "sb_core/cast.h"
#include "sb_core/containers/dynamic_array.h"
#include "sb_core/containers/dynamic_fix_array.h"
#include "sb_core/containers/dynamic_small_array.h"
#include "sb_core/core.h"
#include "sb_core/system.h"
#include "sb_slw/span"
#include "utility.h"

#include <algorithm>
#include <bit>
#include <chrono>
#include <concepts>
#include <conio.h>
#include <cstddef>
#include <iterator>
#include <limits>
#include <cstdio>
#include <sb_core/hash/crc.h>
#include <sb_core/hash/fnv1a.h>
#include <sb_core/hash/hasher.h>
#include <unordered_map>
#include <unordered_set>
#include <sb_core/log.h>
#include <sb_core/error.h>
#include <sb_core/bit.h>
#include <sb_core/io/virtual_file_system.h>
#include <sb_core/io/file_hdl.h>
#include <sb_core/memory/memory_arena.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/string/fix_string.h>
#include <sb_core/hash/hash_str.h>

#include <sb_slw/string_view>
#include <string>
#include <vcruntime_string.h>
#include <wincontypes.h>

using namespace sb;

#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wunused-variable"

struct Vec2i
{
    i32 x = 0;
    i32 y = 0;

    Vec2i & operator-()
    {
        x = -x;
        y = -y;

        return *this;
    }
};

Vec2i operator+(Vec2i lval, Vec2i rval)
{
    return {lval.x + rval.x, lval.y + rval.y};
}

Vec2i operator-(Vec2i lval, Vec2i rval)
{
    return {lval.x - rval.x, lval.y - rval.y};
}

Vec2i vabs(Vec2i val)
{
    return {abs(val.x), abs(val.y)};
}

struct Sprite
{
    char const * data = nullptr;
    Vec2i bounds = {};
    DArray<Vec2i> col[3];

    b32 isNull() const
    {
        return nullptr == data;
    }
};

[[maybe_unused]] static Sprite SPRITES[] = {
    {.data = "####", .bounds = {4, 1}},
    {.data = " # "
             "###"
             " # ",
     .bounds = {3, 3}},
    {.data = "  #"
             "  #"
             "###",
     .bounds = {3, 3}},
    {.data = "#"
             "#"
             "#"
             "#",
     .bounds = {1, 4}},
    {.data = "##"
             "##",
     .bounds = {2, 2}},
};

enum ColType
{
    LEFT,
    RIGHT,
    BOTTOM,
};

ColType getPushType(char val)
{
    return (val == '<') ? ColType::LEFT : ColType::RIGHT;
}

template <typename T>
struct Buffer
{
    DArray<T> data;
    Vec2i exts;

    static Buffer make(Vec2i exts)
    {
        Buffer buffer;
        buffer.data.resize(exts.x * exts.y);
        buffer.exts = exts;

        return buffer;
    }
};

void initSpriteCollisions(Sprite & sprite)
{
    sprite.col[ColType::LEFT].resize(sprite.bounds.y, Vec2i{std::numeric_limits<i32>::max(), std::numeric_limits<i32>::max()});
    sprite.col[ColType::RIGHT].resize(sprite.bounds.y, Vec2i{std::numeric_limits<i32>::min(), std::numeric_limits<i32>::min()});
    sprite.col[ColType::BOTTOM].resize(sprite.bounds.x, Vec2i{std::numeric_limits<i32>::min(), std::numeric_limits<i32>::min()});

    i32 sprite_char_idx = 0;
    for (i32 sprite_y = 0; sprite_y < sprite.bounds.y; ++sprite_y)
    {
        for (i32 sprite_x = 0; sprite_x < sprite.bounds.x; ++sprite_x)
        {
            if (sprite.data[sprite_char_idx] != ' ')
            {
                if (sprite_x > sprite.col[ColType::RIGHT][sprite_y].x)
                {
                    sprite.col[ColType::RIGHT][sprite_y] = Vec2i{sprite_x, sprite_y};
                }
                if (sprite_x < sprite.col[ColType::LEFT][sprite_y].x)
                {
                    sprite.col[ColType::LEFT][sprite_y] = Vec2i{sprite_x, sprite_y};
                }
                if (sprite_y > sprite.col[ColType::BOTTOM][sprite_x].y)
                {
                    sprite.col[ColType::BOTTOM][sprite_x] = Vec2i{sprite_x, sprite_y};
                }
            }
            ++sprite_char_idx;
        }
    }
}

template <typename T>
i32 getBufferLinerIndex(Buffer<T> const & buffer, Vec2i coords)
{
    i32 const linear_idx = coords.y * buffer.exts.x + coords.x;
    sb_assert(buffer.data.size() > (size_t)linear_idx);

    return linear_idx;
}

template <typename T>
Vec2i getBufferCoords(Buffer<T> const & buffer, i32 linear_idx)
{
    Vec2i const coords = {.x = linear_idx % buffer.exts.x, .y = linear_idx / buffer.exts.x};

    return coords;
}

Vec2i spawnRock(slw::span<i32, 7> pile_state, Sprite const & rock)
{
    i32 highest_rock = std::numeric_limits<i32>::min();
    for (i32 const height : pile_state)
    {
        highest_rock = std::max(height, highest_rock);
    }

    return {2, highest_rock + 3 + rock.bounds.y - 1};
}

void initRndrBuffer(Buffer<CHAR_INFO> & buffer)
{
    i32 const row_cnt = buffer.exts.y;
    i32 const col_cnt = buffer.exts.x;

    for (int line_idx = (row_cnt - 1); line_idx != 0; --line_idx)
    {
        for (int col_idx = 0; col_idx != col_cnt; ++col_idx)
        {
            int const char_idx = getBufferLinerIndex(buffer, {col_idx, line_idx});
            if ((col_idx == 0) || (col_idx == (col_cnt - 1)))
            {
                if (line_idx == (row_cnt - 1))
                {
                    buffer.data[char_idx].Char.AsciiChar = '+';
                }
                else
                {
                    buffer.data[char_idx].Char.AsciiChar = '|';
                }
            }
            else if (line_idx == (row_cnt - 1))
            {
                buffer.data[char_idx].Char.AsciiChar = '-';
            }
            else
            {
                buffer.data[char_idx].Char.AsciiChar = '.';
            }
            buffer.data[char_idx].Attributes = 0x71;
        }
    }
}

void copySprite(Buffer<char> & buffer, Sprite const & sprite, Vec2i offset, std::span<i32> max_height)
{
    i32 sprite_char_idx = 0;
    for (i32 sprite_y = 0; sprite_y < sprite.bounds.y; ++sprite_y)
    {
        for (i32 sprite_x = 0; sprite_x < sprite.bounds.x; ++sprite_x)
        {
            Vec2i const sprite_pos = offset + Vec2i{sprite_x, -sprite_y};
            i32 const buffer_idx = getBufferLinerIndex(buffer, sprite_pos);

            if (sprite.data[sprite_char_idx] != ' ')
            {
                buffer.data[buffer_idx] = '#';
                max_height[sprite_x + offset.x] = std::max(max_height[sprite_x + offset.x], sprite_pos.y + 1);
            }
            ++sprite_char_idx;
        }
    }
}

struct SpriteInst
{
    Sprite const * sprite = nullptr;
    Vec2i pos;
};

struct Box
{
    Vec2i top_left = {};
    Vec2i extends = {};
};

bool isPointInBox(Vec2i point, Box box)
{
    return (point.y > (box.top_left.y - box.extends.y)) && (point.y <= box.top_left.y) && (point.x < (box.top_left.x + box.extends.x)) &&
           (point.x >= box.top_left.x);
}

void renderBoard(Buffer<CHAR_INFO> & rndr_buffer, Vec2i rndr_offset, Buffer<char> data_buffer, SpriteInst const * sprite)
{
    i32 const rndr_row_cnt = rndr_buffer.exts.y;
    i32 const rndr_col_cnt = rndr_buffer.exts.x;

    i32 const rndr_bottom_offset = rndr_offset.y - rndr_row_cnt;
    sb_assert(rndr_bottom_offset >= 0);

    bool const display_bottom_line = 0 == rndr_bottom_offset;

    for (int line_idx = (rndr_row_cnt - 1); line_idx >= 0; --line_idx)
    {
        for (int col_idx = 0; col_idx != rndr_col_cnt; ++col_idx)
        {
            int const rndr_char_idx = getBufferLinerIndex(rndr_buffer, {col_idx, line_idx});

            if ((col_idx == 0) || (col_idx == (rndr_col_cnt - 1)))
            {
                if ((line_idx == (rndr_row_cnt - 1)) && display_bottom_line)
                {
                    rndr_buffer.data[rndr_char_idx].Char.AsciiChar = '+';
                }
                else
                {
                    rndr_buffer.data[rndr_char_idx].Char.AsciiChar = '|';
                }
            }
            else if ((line_idx == (rndr_row_cnt - 1)) && display_bottom_line)
            {
                rndr_buffer.data[rndr_char_idx].Char.AsciiChar = '-';
            }
            else
            {
                auto data_offset = Vec2i{0, line_idx};
                if (display_bottom_line)
                {
                    ++data_offset.y;
                }

                Vec2i data_rndr_offset = (rndr_offset - Vec2i{0, 1}) - data_offset;
                data_rndr_offset.x = col_idx - 1;

                char data_char = 0;
                if (nullptr != sprite)
                {
                    Box const sprite_box = {.top_left = sprite->pos, .extends = sprite->sprite->bounds};

                    if (isPointInBox(data_rndr_offset, sprite_box))
                    {
                        auto const sprite_internal_offset = vabs(data_rndr_offset - sprite->pos);
                        char const sprite_data = sprite->sprite->data[sprite_internal_offset.y * sprite->sprite->bounds.x + sprite_internal_offset.x];

                        if (sprite_data != ' ')
                        {
                            data_char = sprite_data;
                        }
                    }
                }

                if (data_char == 0)
                {
                    int const data_char_idx = getBufferLinerIndex(data_buffer, data_rndr_offset);
                    if (data_buffer.data[data_char_idx] != 0)
                    {
                        data_char = '#';
                    }
                    else
                    {
                        data_char = '.';
                    }
                }

                rndr_buffer.data[rndr_char_idx].Char.AsciiChar = data_char;
            }
            rndr_buffer.data[rndr_char_idx].Attributes = 0x71;
        }
    }
}

void clearRndrSprite(Buffer<CHAR_INFO> & buffer, Sprite const & sprite, Vec2i offset)
{
    Vec2i const sprite_rndr_offset = {// +1 because of the left border
                                      .x = offset.x + 1,
                                      // - 2 because exts.y is the number of rows and contains the floor
                                      .y = buffer.exts.y - 2 - offset.y};

    i32 sprite_char_idx = 0;
    for (i32 sprite_y = 0; sprite_y < sprite.bounds.y; ++sprite_y)
    {
        for (i32 sprite_x = 0; sprite_x < sprite.bounds.x; ++sprite_x)
        {
            i32 const buffer_idx = getBufferLinerIndex(buffer, sprite_rndr_offset + Vec2i{sprite_x, sprite_y});

            if (sprite.data[sprite_char_idx] != ' ')
            {
                buffer.data[buffer_idx].Char.AsciiChar = '.';
            }
            ++sprite_char_idx;
        }
    }
}

void writeRndrSprite(Buffer<CHAR_INFO> & buffer, Sprite const & sprite, Vec2i offset, char rock_char = '@')
{
    Vec2i const sprite_rndr_offset = {// +1 because of the left border
                                      .x = offset.x + 1,
                                      // - 2 because exts.y is the number of rows and contains the floor
                                      .y = buffer.exts.y - 2 - offset.y};

    i32 sprite_char_idx = 0;
    for (i32 sprite_y = 0; sprite_y < sprite.bounds.y; ++sprite_y)
    {
        for (i32 sprite_x = 0; sprite_x < sprite.bounds.x; ++sprite_x)
        {
            i32 const buffer_idx = getBufferLinerIndex(buffer, sprite_rndr_offset + Vec2i{sprite_x, sprite_y});

            if (sprite.data[sprite_char_idx] != ' ')
            {
                buffer.data[buffer_idx].Char.AsciiChar = rock_char;
            }
            ++sprite_char_idx;
        }
    }
}

// #pragma optimize( "", off )
bool tryMoveSprite(Sprite const & sprite, Vec2i sprite_pos, Buffer<char> const & board, Vec2i move, ColType colType)
{
    Vec2i new_pos = sprite_pos + move;
    if ((new_pos.x < 0) || ((new_pos.y - (sprite.bounds.y - 1)) < 0))
    {
        return false;
    }
    if ((new_pos.x + sprite.bounds.x) > board.exts.x)
    {
        return false;
    }

    DArray<Vec2i> const & offsets = sprite.col[colType];
    for (auto const & offset : offsets)
    {
        i32 const board_idx = getBufferLinerIndex(board, new_pos + Vec2i{offset.x, -offset.y});

        if (board.data[board_idx] != 0)
        {
            return false;
        }
    }

    // i32 sprite_char_idx = 0;
    // for (i32 sprite_y = 0; sprite_y < sprite.bounds.y; ++sprite_y)
    //{
    //     for (i32 sprite_x = 0; sprite_x < sprite.bounds.x; ++sprite_x)
    //     {
    //         i32 const board_idx = getBufferLinerIndex(board, new_pos + Vec2i{sprite_x, -sprite_y});
    //
    //         if ((sprite.data[sprite_char_idx] != ' ') && (board.data[board_idx] != 0))
    //         {
    //             return false;
    //         }
    //         ++sprite_char_idx;
    //     }
    // }

    return true;
}
// #pragma optimize( "", on)

Vec2i getMove(char val)
{
    return (val == '<') ? Vec2i{-1, 0} : Vec2i{1, 0};
}

// TODO: clean up code
// TODO: move vector code in common
// TODO: profile and try to implement with repetition/cycle detection
void advent::runDay17()
{
    sb_log_info("Running advent day 17 trivia ...\n");

    HANDLE console_hdl = GetStdHandle(STD_OUTPUT_HANDLE);
    sb_assert(nullptr != console_hdl);

    FileHdl file_hdl = vfs::openFileRead("/data/day17.txt");
    sb_assert(file_hdl.isValid());

    FileSize const file_len = vfs::getFileLength(file_hdl);
    MemoryArena const file_buffer = sb_malloc(GHEAP, file_len + 1);
    u8 * const byte_buffer = static_cast<u8 *>(file_buffer.data);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;
    vfs::closeFile(file_hdl);

    slw::string_view file_string_buffer(reinterpret_cast<char const *>(byte_buffer));
    auto tokens_raw = splitString(file_string_buffer, '\n');

    for (auto & token : tokens_raw)
    {
        token = trim(token);
    }

    slw::span<slw::string_view> tokens = tokens_raw;
    if (tokens.back().empty())
    {
        tokens = tokens.subspan(0, tokens.size() - 1);
    }

    for (auto & sprite : SPRITES)
    {
        initSpriteCollisions(sprite);
    }

    [[maybe_unused]] i32 const COL_CNT = 7;
    i32 const ROW_CNT = 1000000;
    i32 const RNDR_ROW_CNT = 60;
    i32 const RNDR_COL_CNT = 9;
    [[maybe_unused]] Buffer<char> pile_data = Buffer<char>::make(Vec2i{7, ROW_CNT});

    COORD cursor_coords = getConsoleCursorPosition(console_hdl);
    auto const y_offset = static_cast<sb::i16>(cursor_coords.Y);

    setConsoleSize(RNDR_ROW_CNT + 20, RNDR_ROW_CNT + y_offset + 5);

    COORD buffer_size = {(short)RNDR_COL_CNT, (short)RNDR_ROW_CNT};
    SMALL_RECT dest = {0, y_offset, (short)RNDR_COL_CNT, static_cast<SHORT>(RNDR_ROW_CNT + y_offset)};
    COORD start_pos = {0, 0};
    Buffer<CHAR_INFO> rndr_buffer = Buffer<CHAR_INFO>::make({RNDR_COL_CNT, RNDR_ROW_CNT});

    initRndrBuffer(rndr_buffer);

    size_t rock_idx = 0;
    i32 max_height[7] = {};
    size_t push_idx = 0;

    bool can_move = true;
    Vec2i const move_down_dir = {0, -1};
    // constexpr size_t MAX_ROCKS = 2022;
    constexpr size_t MAX_ROCKS = 1000000000000;

    Vec2i rndr_offset = {0, RNDR_ROW_CNT};

    SpriteInst rock_inst = {};
    size_t last_max_height = 0;
    size_t push_total_idx = 0;
    i32 sleep_val = 0;
    b8 rndr = false;

    struct RepeatInfo
    {
        size_t highest_val = 0;
        size_t rock_cnt = 0;
        u32 rock_id = 0;
        u32 push_id = 0;
        u32 cnt = 0;
    };
    std::unordered_map<size_t, RepeatInfo> repeats;
    size_t filler_val = 0;

    while (rock_idx < MAX_ROCKS)
    {
        b8 const rndr_bottom = rndr_offset.y == RNDR_ROW_CNT;

        if ((rock_inst.sprite == nullptr) || rock_inst.sprite->isNull())
        {
            rock_inst.sprite = &SPRITES[rock_idx % std::size(SPRITES)];
            rock_inst.pos = spawnRock(max_height, *rock_inst.sprite);

            auto rndr_top = rndr_offset.y - 1;
            if (rndr_bottom)
            {
                rndr_top--;
            }

            if (rock_inst.pos.y > rndr_top)
            {
                rndr_offset.y += rock_inst.pos.y - rndr_top;
            }
        }

        if (rndr)
        {
            renderBoard(rndr_buffer, rndr_offset, pile_data, &rock_inst);
            WriteConsoleOutput(console_hdl, (CHAR_INFO *)rndr_buffer.data.data(), buffer_size, start_pos, &dest);
            Sleep(sleep_val);
        }

        // renderBoard(rndr_buffer, {0, RNDR_ROW_CNT},pile_data,nullptr);
        ++push_total_idx;
        auto const curr_push_val = tokens[0][push_idx];
        Vec2i const move_dir = getMove(curr_push_val);

        can_move = tryMoveSprite(*rock_inst.sprite, rock_inst.pos, pile_data, move_dir, getPushType(curr_push_val));

        if (can_move)
        {
            // clearRndrSprite(rndr_buffer, curr_rock, curr_rock_pos);
            rock_inst.pos = rock_inst.pos + move_dir;
            // writeRndrSprite(rndr_buffer, curr_rock, curr_rock_pos);
            if (rndr)
            {
                renderBoard(rndr_buffer, rndr_offset, pile_data, &rock_inst);
                WriteConsoleOutput(console_hdl, (CHAR_INFO *)rndr_buffer.data.data(), buffer_size, start_pos, &dest);
                Sleep(sleep_val);
            }
        }

        can_move = tryMoveSprite(*rock_inst.sprite, rock_inst.pos, pile_data, move_down_dir, ColType::BOTTOM);
        if (can_move)
        {
            // clearRndrSprite(rndr_buffer, curr_rock, curr_rock_pos);
            rock_inst.pos = rock_inst.pos + move_down_dir;

            if (rndr)
            {
                renderBoard(rndr_buffer, rndr_offset, pile_data, &rock_inst);
            }
            // writeRndrSprite(rndr_buffer, curr_rock, curr_rock_pos);
        }
        // rock settled
        else
        {
            copySprite(pile_data, *rock_inst.sprite, rock_inst.pos, max_height);

            static bool stop_repeat_detect = false;
            if (!stop_repeat_detect && (push_total_idx > (tokens[0].size() * 3)))
            {
                size_t const rock_offset = rock_idx % std::size(SPRITES);
                size_t repeat_id = rock_offset | (integral_cast<size_t>(push_idx) << 32);
                auto const max_val = std::ranges::max_element(max_height);

                auto const iter = repeats.find(repeat_id);
                if (repeats.end() == iter)
                {
                    RepeatInfo const val = {
                        .highest_val = (size_t)*max_val, .rock_cnt = rock_idx, .rock_id = (u32)rock_offset, .push_id = (u32)push_idx, .cnt = 0};

                    repeats.emplace(repeat_id, val);
                }
                else
                {
                    iter->second.cnt++;
                    auto & old_val = iter->second;

                    if (iter->second.cnt < 5)
                    {
                        old_val.highest_val = (size_t)*max_val;
                        old_val.rock_cnt = rock_idx;
                        old_val.rock_id = (u32)rock_offset;
                        old_val.push_id = (u32)push_idx;
                    }
                    else
                    {
                        stop_repeat_detect = true;
                        size_t const height_diff = (size_t)*max_val - old_val.highest_val;
                        size_t const rock_diff = rock_idx - old_val.rock_cnt;
                        size_t const remaining_rocks = (MAX_ROCKS - 1) - rock_idx;
                        size_t const filler_cycle_cnt = remaining_rocks / rock_diff;

                        filler_val = height_diff * filler_cycle_cnt;
                        rock_idx += rock_diff * filler_cycle_cnt;
                    }
                }
            }

            // writeRndrSprite(rndr_buffer, curr_rock, curr_rock_pos, '#');
            ++rock_idx;
            rock_inst = {};
        }

        push_idx = (push_idx + 1) % tokens[0].size();
    }

    if (rndr)
    {
        getchar();
    }
    clearConsole(cursor_coords, ConsoleAttrib{}, console_hdl, RNDR_ROW_CNT, RNDR_COL_CNT);

    auto const max_y = std::ranges::max_element(max_height);
    size_t const top_val = integral_cast<size_t>(*max_y) + integral_cast<size_t>(last_max_height) + filler_val;
    sb_log_info("Max height {}", integral_cast<size_t>(*max_y) + integral_cast<size_t>(last_max_height) + filler_val);
    printf("val: %zu", top_val);

    CloseHandle(console_hdl);
    sb_free(GHEAP, byte_buffer);
}
