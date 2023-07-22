#include "advent.h"
#include "utility.h"

#include <sb_core/log.h>
#include <sb_core/error.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/memory/memory.h>
#include <sb_core/io/virtual_file_system.h>

#include <sb_slw/string_view>

#include <charconv>
#include <unordered_map>
#include <limits>
#include <queue>

using namespace sb;

#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wunused-variable"

struct Vec3
{
    sb::i16 val[3] = {};
};

namespace std
{
    template<>
    struct std::hash<Vec3>
    {
        std::size_t operator()(Vec3 const& val) const noexcept
        {
            return ((std::size_t)val.val[0]) | (((std::size_t)val.val[1]) << 16) | (((std::size_t)val.val[2]) << 32);
        }
    };
}

Vec3 operator + (Vec3 lval, Vec3 rval)
{
    return {(sb::i16)(lval.val[0] + rval.val[0]), (sb::i16)(lval.val[1] + rval.val[1]), (sb::i16)(lval.val[2] + rval.val[2])};
}

bool operator == (Vec3 lval, Vec3 rval)
{
    return (lval.val[0] == rval.val[0]) && (lval.val[1] == rval.val[1]) && (lval.val[2] == rval.val[2]);
}

bool operator <= (Vec3 lval, Vec3 rval)
{
    return (lval.val[0] <= rval.val[0]) && (lval.val[1] <= rval.val[1]) && (lval.val[2] <= rval.val[2]);
}

Vec3 min(Vec3 lval, Vec3 rval)
{
    return {
        std::min(lval.val[0], rval.val[0]),
        std::min(lval.val[1], rval.val[1]),
        std::min(lval.val[2], rval.val[2]),
    };
}

Vec3 max(Vec3 lval, Vec3 rval)
{
    return {
        std::max(lval.val[0], rval.val[0]),
        std::max(lval.val[1], rval.val[1]),
        std::max(lval.val[2], rval.val[2]),
    };
}

unsigned int countSetBits(unsigned int n)
{
    unsigned int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

void advent::runDay18()
{
    sb_log_info("Running advent day 18 trivia ...\n");

    FileHdl file_hdl = vfs::openFileRead("/data/day18.txt");
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

    sb::u32 const SIDE_X_BIT        = 1 << 0;
    sb::u32 const SIDE_NEG_X_BIT    = 1 << 1;
    sb::u32 const SIDE_Y_BIT        = 1 << 2;
    sb::u32 const SIDE_NEG_Y_BIT    = 1 << 3;
    sb::u32 const SIDE_Z_BIT        = 1 << 4;
    sb::u32 const SIDE_NEG_Z_BIT    = 1 << 5;

    struct Cube
    {
        Vec3 pos;
        sb::u32 ext_side = 0U;
    };

    struct Cell
    {
        Vec3 pos;
        bool visited = false;
    };

    std::unordered_map<Vec3, int> cubes_map;
    std::unordered_map<Vec3, int> empty_cells_map;
    sb::DArray<Cube> cubes;
    sb::DArray<Cell> cells;
    int cube_cnt = 0;
    cubes.reserve(tokens.size());

    Vec3 min_pos{std::numeric_limits<sb::i16>::max(),
        std::numeric_limits<sb::i16>::max(),
        std::numeric_limits<sb::i16>::max()};

    Vec3 max_pos{std::numeric_limits<sb::i16>::min(),
        std::numeric_limits<sb::i16>::min(),
        std::numeric_limits<sb::i16>::min()};

    for (auto const& token : tokens)
    {
        auto const pos_components = splitString(token, ',');
        Cube cube;

        std::from_chars(pos_components[0].data(), pos_components[0].data() + pos_components[0].size(), cube.pos.val[0]);
        std::from_chars(pos_components[1].data(), pos_components[1].data() + pos_components[1].size(), cube.pos.val[1]);
        std::from_chars(pos_components[2].data(), pos_components[2].data() + pos_components[2].size(), cube.pos.val[2]);

        if (cubes_map.contains(cube.pos))
        {
            continue;
        }

        cubes.push_back(cube);
        cubes_map.emplace(cube.pos, cube_cnt);

        min_pos = min(min_pos, cube.pos);
        max_pos = max(max_pos, cube.pos);

        ++cube_cnt;
    }    

    struct SideInfo
    {
        Vec3 dir;
        sb::u32 border_bit;
        sb::u32 inv_border_bit;
    };

    SideInfo const SIDES_INFO[] =
    {
        {{1, 0, 0}, SIDE_X_BIT,     SIDE_NEG_X_BIT},
        {{-1, 0, 0},SIDE_NEG_X_BIT, SIDE_X_BIT},
        {{0, 1, 0}, SIDE_Y_BIT,     SIDE_NEG_Y_BIT},
        {{0, -1, 0},SIDE_NEG_Y_BIT, SIDE_Y_BIT},
        {{0, 0, 1}, SIDE_Z_BIT,     SIDE_NEG_Z_BIT},
        {{0, 0, -1},SIDE_NEG_Z_BIT, SIDE_Z_BIT},
    };

    for (auto &cube : cubes)
    {        
        for (auto const &side_info : SIDES_INFO)
        {
            Vec3 side_pos = side_info.dir + cube.pos;
            auto const iter = cubes_map.find(side_pos);
            if (iter == cubes_map.end())
            {
                cube.ext_side |= side_info.border_bit;
            }
        }
    }

    sb::u32 total_surface = 0;
    for (Cube const & cube : cubes)
    {
        total_surface += countSetBits(cube.ext_side);
    }

    cells.reserve(6000);

    for (int x = min_pos.val[0] ; x <= max_pos.val[0]; ++x)
    {
        for (int y = min_pos.val[1] ; y <= max_pos.val[1]; ++y)
        {
            for (int z = min_pos.val[2] ; z <= max_pos.val[2]; ++z)
            {
                Vec3 pos{(sb::i16)x,(sb::i16)y,(sb::i16)z};
                if (!cubes_map.contains(pos))
                {
                    empty_cells_map.emplace(pos, cells.size());
                    cells.push_back(Cell{pos, false});
                }
            }
        }
    }

    for (Cell &cell : cells)
    {
        if (!cell.visited)
        {
            sb::DArray<std::pair<int, sb::u32>> hit_cubes;
            bool hit_world_border = false;

            std::queue<Cell> cell_queue;
            cell.visited = true;
            cell_queue.push(cell);

            while (!cell_queue.empty())
            {
                Cell curr_cell = cell_queue.front();
                cell_queue.pop();

                for (auto const &side_info : SIDES_INFO)
                {
                    Vec3 side_pos = side_info.dir + curr_cell.pos;
                    if ((min_pos <= side_pos) && (side_pos <= max_pos))
                    {
                        auto const cube_iter = cubes_map.find(side_pos);
                        if (cube_iter != cubes_map.end())
                        {
                            hit_cubes.push_back(std::pair<int, sb::u32>{cube_iter->second, side_info.inv_border_bit});
                        }
                        else
                        {
                            auto const next_cell_desc = empty_cells_map.find(side_pos);
                            sb_assert(next_cell_desc != empty_cells_map.end());

                            Cell & next_cell = cells[next_cell_desc->second];
                            if (!next_cell.visited)
                            {
                                next_cell.visited = true;
                                cell_queue.push(next_cell);
                            }
                        }
                    }
                    else
                    {
                        hit_world_border = true;
                    }
                }
            }

            if (!hit_world_border)
            {
                for (auto const & hit_cube : hit_cubes)
                {
                    cubes[hit_cube.first].ext_side &= ~hit_cube.second;
                }
            }
        }
    }
    
    sb::u32 external_surface = 0;
    for (Cube const & cube : cubes)
    {
        external_surface += countSetBits(cube.ext_side);
    }

    sb_log_info("Total surface: {} units", total_surface);
    sb_log_info("External surface {} units", external_surface);

    sb_free(GHEAP, byte_buffer);
}
