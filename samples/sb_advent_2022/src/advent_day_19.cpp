#include "advent.h"
#include "sb_core/containers/dynamic_array.h"
#include "sb_core/core.h"
#include "utility.h"

#include <random>
#include <sb_core/log.h>
#include <sb_core/error.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/memory/memory.h>
#include <sb_core/io/virtual_file_system.h>
#include <sb_core/containers/array.h>

#include <sb_slw/string_view>

#include <charconv>
#include <cstdio>
#include <sys/stat.h>
#include <unordered_map>
#include <algorithm>
#include <winnt.h>
#include <xutility>
#include <ranges>
#include <queue>
#include <deque>
#include <stack>

using namespace sb;

#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wunused-variable"

enum Resource : u32
{
    ORE,
    CLAY,
    OBSIDIAN,
    GEODE,
    COUNT
};

const u32 RESOURCE_MASK = 0b1111;

struct ResourceStatus
{
    i32 val[(int)Resource::COUNT] = {};

    ResourceStatus & sub(ResourceStatus const & rsc)
    {
        val[0] -= rsc.val[0];
        val[1] -= rsc.val[1];
        val[2] -= rsc.val[2];
        val[3] -= rsc.val[3];

        return *this;
    }

    ResourceStatus & add(ResourceStatus const & rsc)
    {
        val[0] += rsc.val[0];
        val[1] += rsc.val[1];
        val[2] += rsc.val[2];
        val[3] += rsc.val[3];

        return *this;
    }

    bool isNeg() const
    {
        return (val[0] < 0) || (val[1] < 0) || (val[2] < 0) || (val[3] < 0);
    }

    bool isEmpty() const
    {
        return (val[0] == 0) && (val[1] == 0) && (val[2] == 0) && (val[3] == 0);
    }
};

int div_ceil(int x, int y)
{
    return x / y + (x % y > 0);
}

bool operator<=(ResourceStatus const & lval, ResourceStatus const & rval)
{
    return (lval.val[0] <= rval.val[0]) && (lval.val[1] <= rval.val[1]) && (lval.val[2] <= rval.val[2]) && (lval.val[3] <= rval.val[3]);
}

ResourceStatus operator-(ResourceStatus const & lval, ResourceStatus const & rval)
{
    return {(lval.val[0] - rval.val[0]), (lval.val[1] - rval.val[1]), (lval.val[2] - rval.val[2]), (lval.val[3] - rval.val[3])};
}

bool operator>=(ResourceStatus const & lval, ResourceStatus const & rval)
{
    return (lval.val[0] >= rval.val[0]) && (lval.val[1] >= rval.val[1]) && (lval.val[2] >= rval.val[2]) && (lval.val[3] >= rval.val[3]);
}

bool operator==(ResourceStatus const & lval, ResourceStatus const & rval)
{
    return (lval.val[0] == rval.val[0]) && (lval.val[1] == rval.val[1]) && (lval.val[2] == rval.val[2]) && (lval.val[3] == rval.val[3]);
}

bool operator/(ResourceStatus const & lval, ResourceStatus const & rval)
{
    return (lval.val[0] / rval.val[0]) && (lval.val[1] / rval.val[1]) && (lval.val[2] / rval.val[2]) && (lval.val[3] / rval.val[3]);
}

void clampZero(ResourceStatus & val)
{
    val.val[0] = std::max(0, val.val[0]);
    val.val[1] = std::max(0, val.val[1]);
    val.val[2] = std::max(0, val.val[2]);
    val.val[3] = std::max(0, val.val[3]);
}

u32 getResourceMask(ResourceStatus const & status)
{
    u32 mask = 0U;

    u32 idx = 0U;
    for (auto val : status.val)
    {
        if (val)
        {
            mask |= 1 << idx;
        }
        ++idx;
    }

    return mask;
}

struct Status
{
    ResourceStatus robots = {};
    ResourceStatus resources = {};
    u32 robot_mask = 0;
    u32 time = 0;
};

struct RobotDesc
{
    ResourceStatus cost;
    sb::u32 mask = 0;
};

struct BluePrint
{
    i32 id = 0;
    sb::Array<RobotDesc, (int)Resource::COUNT> robots = {};
};

template <typename T, typename Y>
Y removeSwap(T & container, Y & iter)
{
    if (iter == (container.end() - 1))
    {
        return container.erase(iter);
    }

    auto const dist = std::distance(container.begin(), iter);
    std::swap(*iter, *(container.end() - 1));
    container.resize(container.size() - 1);
    return container.begin() + dist;
}

Resource getResourceType(std::string_view name)
{
    if (name == "ore")
    {
        return Resource::ORE;
    }
    else if (name == "clay")
    {
        return Resource::CLAY;
    }
    else if (name == "obsidian")
    {
        return Resource::OBSIDIAN;
    }
    else
    {
        sb_assert(name == "geode");
        return Resource::GEODE;
    }

    sb_unreacheable_code();
}

void advent::runDay19()
{
    sb_log_info("Running advent day 19 trivia ...\n");

    FileHdl file_hdl = vfs::openFileRead("/data/day19.txt");
    sb_assert(file_hdl.isValid());

    FileSize const file_len = vfs::getFileLength(file_hdl);
    MemoryArena const file_buffer = sb_malloc(GHEAP, file_len + 1);
    u8 * const byte_buffer = static_cast<u8 *>(file_buffer.data);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;
    vfs::closeFile(file_hdl);

    slw::string_view file_string_buffer(reinterpret_cast<char const *>(byte_buffer));
    auto tokens_raw = splitString(file_string_buffer, '\n');

    sb::DArray<BluePrint> blueprints;

    for (auto & token : tokens_raw)
    {
        token = trim(token);
        BluePrint blueprint;

        auto robots_cost_items = splitString(token, '.');
        for (auto & robot_cost : robots_cost_items)
        {
            if (!robot_cost.empty())
            {
                auto const sub_tokens = splitString(robot_cost, ' ');
                std::from_chars(sub_tokens[1].data(), sub_tokens[1].data() + sub_tokens[1].size(), blueprint.id);

                auto sub_token_span = std::span{sub_tokens};
                auto cost_iter = std::ranges::find(sub_token_span, "costs");
                sb_assert(cost_iter != end(sub_token_span));

                Resource const robot_type = getResourceType(*(cost_iter - 2));
                ResourceStatus rsc_status = {};

                sb::u32 rsc_mask = 0U;
                do
                {
                    cost_iter++;
                    int rsc_cnt = 0;
                    std::from_chars(cost_iter->data(), cost_iter->data() + cost_iter->size(), rsc_cnt);
                    Resource const rsc_type = getResourceType(*(cost_iter + 1));

                    rsc_status.val[(int)rsc_type] = rsc_cnt;
                    rsc_mask |= 1 << rsc_type;

                    sub_token_span = sub_token_span.subspan(std::distance(begin(sub_token_span), cost_iter + 1));
                    cost_iter = std::ranges::find(sub_token_span, "and");
                } while (cost_iter != end(sub_token_span));

                auto & robot_desc = blueprint.robots[(int)robot_type];
                robot_desc.cost = rsc_status;
                robot_desc.mask = rsc_mask;
            }
        }

        blueprints.push_back(blueprint);
    }

    std::stack<Status> statuses;


    std::vector<i32> blueprint_values;
    blueprint_values.reserve(blueprints.size());

    const auto blueprint_span = std::span(std::data(blueprints), std::min((int)blueprints.size(), 3));
    sb_log_info("nb blueprint {}", blueprint_span.size());

    u32 blueprint_idx = 0; 
    for (auto const & curr_blueprint : blueprint_span)
    {
        i32 max_geodes = 0;
        Status init_status;
        init_status.robots.val[Resource::ORE] = 1;
        init_status.robot_mask = getResourceMask(init_status.robots);
        statuses.push(init_status);
        ResourceStatus empty_rsc = {};
        constexpr u32 MAX_TIME = 32;

        while (!statuses.empty())
        {
            auto curr_status = statuses.top();
            statuses.pop();

            if (max_geodes != 0)
            {
                i32 const remaining_time = (MAX_TIME - (i32)curr_status.time);
                i32 val = curr_status.resources.val[Resource::GEODE] + curr_status.robots.val[Resource::GEODE] * remaining_time;
                val += remaining_time * (remaining_time + 1)/2;

                if (val < max_geodes)
                {
                    continue;
                }
            }

            u32 const robot_mask = curr_status.robot_mask;
            u32 robot_idx = 0;
            for (auto const & robot : curr_blueprint.robots)
            {
                const u32 curr_robot_mask = 1 << robot_idx;
                u32 const rsc_mask = robot.mask;

                if (rsc_mask == (robot_mask & rsc_mask))
                {
                    ResourceStatus rsc_diff = robot.cost - curr_status.resources;
                    clampZero(rsc_diff);

                    // can buy it now
                    if (rsc_diff.isEmpty() && !curr_status.resources.isEmpty())
                    {
                        Status new_status = curr_status;

                        new_status.time++;
                        new_status.resources.sub(robot.cost);
                        new_status.robot_mask |= curr_robot_mask;

                        for (u32 idx = 0; idx != Resource::COUNT; idx++)
                        {
                            new_status.resources.val[idx] += new_status.robots.val[idx];
                        }

                        new_status.robots.val[robot_idx]++;

                        if (new_status.time >= MAX_TIME)
                        {
                            max_geodes = std::max(new_status.resources.val[Resource::GEODE], max_geodes);
                            break;
                        }

                        statuses.push(new_status);

                        sb_assert(!new_status.resources.isNeg());

                        robot_idx++;
                        continue;
                    }

                    i32 max_diff = 0;
                    i32 rb_cnt_idx = 0;
                    for (auto const & diff : rsc_diff.val)
                    {
                        i32 const rb_cnt = curr_status.robots.val[rb_cnt_idx];
                        if (rb_cnt)
                        {
                            max_diff = std::max(max_diff, div_ceil(diff, rb_cnt));
                        }
                        rb_cnt_idx++;
                    }

                    Status new_status = curr_status;
                    i32 const remaining_time = MAX_TIME - (i32)curr_status.time;


                    if ((curr_status.time + max_diff) < MAX_TIME)
                    {
                        new_status.time += (max_diff + 1);
                        for (u32 idx = 0; idx != Resource::COUNT; idx++)
                        {
                            new_status.resources.val[idx] += new_status.robots.val[idx] * (max_diff + 1);
                        }

                        new_status.resources.sub(robot.cost);
                        sb_assert(!new_status.resources.isNeg());
                        new_status.robots.val[robot_idx]++;
                        new_status.robot_mask |= curr_robot_mask;

                        if (MAX_TIME == new_status.time)
                        {
                            max_geodes = std::max(new_status.resources.val[Resource::GEODE], max_geodes);
                        }
                        else
                        {
                            statuses.push(new_status);
                        }
                    }
                    else
                    {
                        for (u32 idx = 0; idx != Resource::COUNT; idx++)
                        {
                            new_status.resources.val[idx] += new_status.robots.val[idx] * remaining_time;
                        }

                        max_geodes = std::max(new_status.resources.val[Resource::GEODE], max_geodes);
                    }
                }
                ++robot_idx;
            }
        }

        sb_log_info("max geodes {} : {}", blueprint_idx, max_geodes);

        blueprint_values.push_back(max_geodes);
        max_geodes = 0;
        ++blueprint_idx;
    }

    u32 total_res = 0;
    u32 idx = 1;
    for (auto const& val : blueprint_values)
    {
        total_res += idx*val;
        ++idx;
    }

    u64 total_res2 = 1;
    for (auto const& val : blueprint_values)
    {
        total_res2 *= (u64)val;
    }

    sb_log_info("result: {}", total_res);
    sb_log_info("result2: {}", total_res2);

    sb_log_info("Job done");

    sb_free(GHEAP, byte_buffer);
}
