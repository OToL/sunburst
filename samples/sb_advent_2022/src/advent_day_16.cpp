#include "advent.h"
#include "sb_core/containers/dynamic_array.h"
#include "sb_core/containers/dynamic_fix_array.h"
#include "sb_core/containers/dynamic_small_array.h"
#include "sb_core/core.h"
#include "utility.h"

#include <bit>
#include <limits>
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

#include <charconv>
#include <algorithm>
#include <system_error>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <memory>
#include <stack>
#include <xutility>

using slw::distance;

using namespace sb;

struct Edge
{
    slw::string_view name_str;
    i32 dest = 0;
    i32 cost = 0;
};

struct Node
{
    slw::string_view name_str;
    sb::HashStr name;
    i32 flow_rate = 0;
    DArray<Edge> neighbors;
};

using Graph = DArray<Node>;
[[maybe_unused]] i32 constexpr VISIT_COUNT_LIMIT = 3;

struct PathContext
{
    PathContext(Graph const & graph)
    {
        nodes.reserve(graph.size() - 1);
    }

    DArray<i32> nodes;
    u64 visited_nodes = 0ULL;
};

struct PathContext2
{
    PathContext2(Graph const & graph)
        : unused_valves((1ULL << graph.size()) - 1)
    {
        nodes.reserve(graph.size() - 1);
        --unused_valves; // deactive start node
    }

    DArray<Edge> nodes;
    u64 unused_valves = 0ULL;
    i32 time = 0;
    i32 curr_flow_rate = 0;
    i32 total_pressure = 0;
};

struct TargetNode
{
    Edge edge;
    i32 remaining_cost = 0;
};

struct DualPathContext
{
    DualPathContext(Graph const & graph)
        : unused_valves((1ULL << graph.size()) - 1)
    {
        nodes[0].reserve(graph.size() - 1);
        nodes[1].reserve(graph.size() - 1);

        --unused_valves; // deactive start node
    }

    b8 isCurrTarget(i32 target_idx) const
    {
        for (auto const & node : nodes)
        {
            if (node.back().edge.dest == target_idx)
            {
                return true;
            }
        }

        return false;
    }

    using TargetNodes = DArray<TargetNode>;

    TargetNodes nodes[2];
    i32 time = 0;
    u64 unused_valves = 0ULL;
    i32 curr_flow_rate = 0;
    i32 total_pressure = 0;
};

void findBestDualPath(Graph const & graph, DualPathContext & curr_path, DualPathContext & best_path, i32 time_limit)
{
    for (auto const & nodes : curr_path.nodes)
    {
        if (nodes.back().remaining_cost == 0)
        {
            TargetNode const curr_edge = nodes.back();
            auto const curr_node_idx = curr_edge.edge.dest;
            // Node const & curr_node = graph[curr_node_idx];

            curr_path.unused_valves &= ~(1ULL << (u64)curr_node_idx);
        }
    }

    auto remaining_time = time_limit - curr_path.time;
    if (curr_path.unused_valves == 0)
    {
        curr_path.total_pressure += (curr_path.curr_flow_rate * remaining_time);
        remaining_time = 0;
    }

    if (remaining_time == 0)
    {
        if ((best_path.total_pressure == 0) || curr_path.total_pressure > best_path.total_pressure)
        {
            best_path = curr_path;
        }

        return;
    }

    DSArray<i32, 20> neighbors;
    i32 remaing_flow = 0;
    for (size_t node_idx = 0; node_idx < graph.size(); ++node_idx)
    {
        auto const node_mask = (1ULL << (u64)node_idx);
        if ((0 != (curr_path.unused_valves & node_mask)) && !curr_path.isCurrTarget((i32)node_idx))
        {
            remaing_flow += graph[node_idx].flow_rate;
            neighbors.push_back((i32)node_idx);
        }
    }

    // Early exit
    if ((best_path.total_pressure != 0) && ((curr_path.total_pressure + (curr_path.curr_flow_rate + remaing_flow)*remaining_time) < best_path.total_pressure))
    {
        return;
    }

    if (neighbors.empty())
    {
        sb_assert((curr_path.nodes[0].back().remaining_cost == 0) || (curr_path.nodes[1].back().remaining_cost == 0));
        for (auto & nodes : curr_path.nodes)
        {
            auto const remaining_cost = nodes.back().remaining_cost;
            if (remaining_cost > 0)
            {
                Node const & dest_node = graph[nodes.back().edge.dest];
                if ((curr_path.time + remaining_cost) < time_limit)
                {
                    curr_path.total_pressure += remaining_cost * curr_path.curr_flow_rate;
                    curr_path.curr_flow_rate += dest_node.flow_rate;
                    curr_path.time += remaining_cost;
                }

                nodes.back().remaining_cost = 0;

                break;
            }
        }

        findBestDualPath(graph, curr_path, best_path, time_limit);
    }
    else
    {
        struct Move
        {
            i32 l_dest = -1;
            i32 r_dest = -1;

            i32 operator[](u32 idx) const
            {
                if (0 == idx)
                {
                    return l_dest;
                }

                return r_dest;
            }

            i32 & operator[](u32 idx)
            {
                if (0 == idx)
                {
                    return l_dest;
                }

                return r_dest;
            }
        };

        bool const is_done[] = {curr_path.nodes[0].back().remaining_cost == 0, curr_path.nodes[1].back().remaining_cost == 0};

        DSArray<Move, 90> moves;
        if (neighbors.size() == 1)
        {
            for (auto const & neigh_idx : neighbors)
            {
                if (is_done[0])
                {
                    moves.push_back(Move{neigh_idx, -1});
                }
                if (is_done[1])
                {
                    moves.push_back(Move{-1, neigh_idx});
                }
            }
        }
        else
        {
            if (is_done[0] && is_done[1])
            {
                for (size_t neigh_offset = 0; neigh_offset < neighbors.size(); ++neigh_offset)
                {
                    for (size_t neigh_offset2 = 0; neigh_offset2 < neighbors.size(); ++neigh_offset2)
                    {
                        if (neigh_offset != neigh_offset2)
                        {
                            moves.push_back(Move{neighbors[neigh_offset], neighbors[neigh_offset2]});
                        }
                    }
                }
            }
            else
            {
                i32 const moving_idx = is_done[0] ? 0 : 1;

                for (i32 neighbor : neighbors)
                {
                    Move new_move;
                    new_move[moving_idx] = neighbor;

                    moves.push_back(new_move);
                }
            }
            // else if (is_done[0])
            //{
            //     moves.push_back(Move{neighbors[neigh_offset], -1});
            // }
            // else
            //{
            //     moves.push_back(Move{-1, neighbors[neigh_offset]});
            // }
        }

        for (auto const & move : moves)
        {
            DualPathContext new_path(curr_path);

            i32 min_cost = std::numeric_limits<i32>::max();

            for (i32 move_idx = 0; move_idx < 2; ++move_idx)
            {
                if (move[move_idx] != -1)
                {
                    auto const target_node_idx = move[move_idx];
                    Node const & curr_node = graph[new_path.nodes[move_idx].back().edge.dest];
                    auto const iter =
                        std::ranges::find_if(curr_node.neighbors, [target_node_idx](auto const & edge) { return target_node_idx == edge.dest; });
                    sb_assert(iter != end(curr_node.neighbors));

                    new_path.nodes[move_idx].push_back(TargetNode{*iter, iter->cost + 1});
                }

                i32 const remaining_cost = new_path.nodes[move_idx].back().remaining_cost;
                if (0 != remaining_cost)
                {
                    min_cost = std::min(min_cost, remaining_cost);
                }
            }

            sb_assert(min_cost != std::numeric_limits<i32>::max());

            if ((new_path.time + min_cost) < time_limit)
            {
                new_path.total_pressure += min_cost * new_path.curr_flow_rate;

                for (i32 move_idx = 0; move_idx < 2; ++move_idx)
                {
                    auto & curr_target = new_path.nodes[move_idx].back();
                    if (curr_target.remaining_cost == min_cost)
                    {
                        new_path.curr_flow_rate += graph[curr_target.edge.dest].flow_rate;
                        curr_target.remaining_cost = 0;
                    }
                    else if (0 != curr_target.remaining_cost)
                    {
                        curr_target.remaining_cost -= min_cost;
                    }
                }

                new_path.time += min_cost;
            }
            else
            {
                new_path.unused_valves = 0;
            }

            findBestDualPath(graph, new_path, best_path, time_limit);
        }
    }
}

void findBestPath(Graph const & graph, PathContext2 & curr_path, PathContext2 & best_path, i32 time_limit)
{
    Edge const curr_edge = curr_path.nodes.back();
    auto const curr_node_idx = curr_edge.dest;
    Node const & curr_node = graph[curr_node_idx];

    curr_path.unused_valves &= ~(1ULL << (u64)curr_node_idx);
    auto remaining_time = time_limit - curr_path.time;
    if (curr_path.unused_valves == 0)
    {
        curr_path.total_pressure += (curr_path.curr_flow_rate * remaining_time);
        remaining_time = 0;
    }

    if (remaining_time == 0)
    {
        if ((best_path.total_pressure == 0) || curr_path.total_pressure > best_path.total_pressure)
        {
            best_path = curr_path;
        }

        return;
    }

    DSArray<Edge, 10> neighbors;
    for (auto const & link : curr_node.neighbors)
    {
        auto const link_mask = (1ULL << (u64)link.dest);
        if (0 != (curr_path.unused_valves & link_mask))
        {
            neighbors.push_back(link);
        }
    }

    auto neighbor_cnt = neighbors.size();
    PathContext2 sub_path(graph);

    while (neighbor_cnt != 0)
    {
        PathContext2 * new_path = &curr_path;
        auto const & edge = neighbors[neighbor_cnt - 1];

        if (neighbor_cnt != 1)
        {
            sub_path = curr_path;
            new_path = &sub_path;
        }

        if ((curr_path.time + edge.cost + 1) >= time_limit)
        {
            new_path->unused_valves = 0;
            findBestPath(graph, *new_path, best_path, time_limit);
        }
        else
        {
            new_path->total_pressure += (curr_path.curr_flow_rate * (edge.cost + 1));
            new_path->time += (edge.cost + 1);
            new_path->curr_flow_rate += graph[edge.dest].flow_rate;
            new_path->nodes.push_back(edge);

            findBestPath(graph, *new_path, best_path, time_limit);
        }

        neighbor_cnt--;
    }
}

void findShortestPathRecurse(Graph const & graph, HashStr lnode, HashStr rnode, PathContext & path, PathContext & shortest_path)
{
    auto const curr_node_idx = path.nodes.back();
    Node const & curr_node = graph[curr_node_idx];
    u64 const curr_node_mask = 1ULL << curr_node_idx;

    // node already visited ...
    if (0 != (path.visited_nodes & curr_node_mask))
    {
        return;
    }

    // longer than the best one ...
    if (!shortest_path.nodes.empty() && (shortest_path.nodes.size() < path.nodes.size()))
    {
        return;
    }

    // far too long
    if (path.nodes.size() == path.nodes.capacity())
    {
        return;
    }

    if (curr_node.name == rnode)
    {
        shortest_path = path;
        return;
    }

    path.visited_nodes |= curr_node_mask;

    DSArray<i32, 10> actual_neighbor;
    for (auto const & edge : curr_node.neighbors)
    {
        u64 const neighbor_node_mask = 1ULL << edge.dest;

        if (0 == (path.visited_nodes & neighbor_node_mask))
        {
            actual_neighbor.push_back(edge.dest);
        }
    }

    auto neighbor_cnt = actual_neighbor.size();
    while (neighbor_cnt != 0)
    {
        auto const neighbor_idx = actual_neighbor[neighbor_cnt - 1];
        if (neighbor_cnt == 1)
        {
            path.nodes.push_back(neighbor_idx);
            findShortestPathRecurse(graph, lnode, rnode, path, shortest_path);
        }
        else
        {
            PathContext new_path(graph);
            new_path = path;
            new_path.nodes.push_back(neighbor_idx);
            findShortestPathRecurse(graph, lnode, rnode, new_path, shortest_path);
        }

        neighbor_cnt--;
    }
}

void findShortestPath(Graph const & graph, HashStr lnode, HashStr rnode, PathContext & shortest_path)
{
    PathContext base_path(graph);

    auto const node_iter = slw::find_if(begin(graph), end(graph), [lnode](Node const & node) { return node.name == lnode; });
    sb_assert(node_iter != end(graph));

    base_path.nodes.push_back((i32)slw::distance(begin(graph), node_iter));

    findShortestPathRecurse(graph, lnode, rnode, base_path, shortest_path);
}

// Solution
//   1728
//   2304
void advent::runDay16()
{
    sb_log_info("Running advent day 16 trivia ...\n");

    FileHdl file_hdl = vfs::openFileRead("/data/day16.txt");
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

    // Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
    DArray<Node> graph;
    graph.reserve(tokens.size());
    for (auto const & token : tokens)
    {
        Node new_node;
        new_node.name_str = token.substr(strlen("Value "), 2);
        new_node.name = sb::HashStr::make(new_node.name_str.data(), new_node.name_str.size());
        auto res = std::from_chars(token.data() + strlen("Valve XX has flow rate="), token.data() + token.size(), new_node.flow_rate);
        sb_assert(res.ec == std::errc());

        graph.push_back(new_node);
    }

    for (auto const & token : tokens)
    {
        HashStr const name = sb::HashStr::make(token.data() + strlen("Value "), 2);
        auto const node_iter = slw::find_if(begin(graph), end(graph), [&name](Node const & node) { return node.name == name; });
        sb_assert(node_iter != end(graph));

        auto const node_idx = slw::distance(begin(graph), node_iter);

        auto valves_offset = token.find("valve");
        sb_assert(valves_offset != slw::string_view::npos);

        valves_offset += strlen("valve");
        if (token[valves_offset] == 's')
        {
            valves_offset++;
        }

        auto const valves_str = trim(token.substr(valves_offset));
        auto const valve_nodes = splitString(valves_str, ", ");

        for (auto valve_node : valve_nodes)
        {
            valve_node = trim(valve_node);
            HashStr const valve_hash = HashStr::make(valve_node.data(), valve_node.size());
            auto connection_iter = slw::find_if(begin(graph), end(graph), [&valve_hash](Node const & node) { return node.name == valve_hash; });
            sb_assert(connection_iter != end(graph));

            graph[node_idx].neighbors.push_back({valve_node, (i32)distance(begin(graph), connection_iter), 1});
        }
    }

    auto const start_graph_iter = std::ranges::find_if(graph, [](Node const & node) { return node.name_str == "AA"; });
    sb_assert(end(graph) != start_graph_iter);
    sb_assert(start_graph_iter->flow_rate == 0);

    Graph dag;
    dag.reserve(graph.size());

    Node start_node;
    start_node.name_str = start_graph_iter->name_str;
    start_node.name = start_graph_iter->name;
    start_node.flow_rate = start_graph_iter->flow_rate;
    dag.push_back(start_node);

    for (Node const & node : graph)
    {
        if (node.flow_rate != 0)
        {
            Node new_node;
            new_node.name_str = node.name_str;
            new_node.name = node.name;
            new_node.flow_rate = node.flow_rate;
            dag.push_back(new_node);
        }
    }

    for (Node & node : dag)
    {
        i32 dest_node_idx = 0;
        for (Node const & dest_node : dag)
        {
            if ((node.name != dest_node.name) && (dest_node_idx != 0))
            {
                PathContext shortest_path(graph);

                findShortestPath(graph, node.name, dest_node.name, shortest_path);
                sb_assert(!shortest_path.nodes.empty());

                Edge edge;
                edge.name_str = dest_node.name_str;
                edge.dest = dest_node_idx;
                edge.cost = (i32)(shortest_path.nodes.size() - 1);
                node.neighbors.push_back(edge);
            }

            dest_node_idx++;
        }
    }

    PathContext2 curr_path(dag), best_path(dag);
    Edge start_edge = {.name_str = "AA", .dest = 0, .cost = 0};
    curr_path.nodes.push_back(start_edge);

    findBestPath(dag, curr_path, best_path, 30);
    sb_log_info("Last flow rate {}", best_path.curr_flow_rate);
    sb_log_info("Best path {}", best_path.total_pressure);

    DualPathContext curr_path2(dag);
    DualPathContext best_path2(dag);
    TargetNode start_edge2 = {.edge = {.name_str = "AA", .dest = 0, .cost = 0}, .remaining_cost = 0};

    curr_path2.nodes[0].push_back(start_edge2);
    curr_path2.nodes[1].push_back(start_edge2);

    findBestDualPath(dag, curr_path2, best_path2, 26);
    sb_log_info("Team flow rate {}", best_path2.total_pressure);

    sb_free(GHEAP, byte_buffer);
}
