#include "advent.h"
#include "sb_core/core.h"
#include "utility.h"

#include <charconv>
#include <cstdlib>
#include <iterator>
#include <sb_core/log.h>
#include <sb_core/error.h>
#include <sb_core/string/utility.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/io/virtual_file_system.h>
#include <sb_core/containers/dynamic_array.h>

#include <sb_slw/string_view>
#include <sb_slw/span>
#include <winnt.h>

using namespace sb;

#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"

struct ValueNode
{
    i64 value = 0;
    i64 prev = -1;
    i64 next = -1;
};

i64 getNthItem(ValueNode const * startNode, u32 count, slw::span<ValueNode> const & values)
{
    u64 idx = 0;
    auto iter = startNode;
    while (count != idx)
    {
        iter = &values[iter->next];
        ++idx;
    }

    return iter->value;
}

void dbgPrintList(ValueNode const * begin, slw::span<ValueNode> const & values)
{
    sb_log_info("Dbg list output:");
    ValueNode const * iter = begin;

    do
    {
        sb_log_info("{}", iter->value);
        iter = &values[iter->next];
    } while (iter != begin);
}

void dbgPrintList(ValueNode const * begin, ValueNode const * end, slw::span<ValueNode> const & values)
{
    ValueNode const * iter = begin;

    sb_log_info("Dbg list output:");
    while (iter != end)
    {
        sb_log_info("{}", iter->value);
        iter = &values[iter->next];
    }
    sb_log_info("{}", iter->value);
}

void unlinkNode(ValueNode & node, slw::span<ValueNode> const & nodes)
{
    nodes[node.prev].next = node.next;
    nodes[node.next].prev = node.prev;
}

void linkNodeAfter(ValueNode & node, ValueNode & target_node, slw::span<ValueNode> const & nodes)
{
    const i64 node_idx = (i64)std::distance(nodes.data(), &node);
    i64 next_node = target_node.next;
    target_node.next = node_idx;
    nodes[next_node].prev = node_idx;
    node.next = next_node;
    node.prev = (i64)std::distance(nodes.data(), &target_node);
}

void linkNodeBefore(ValueNode & node, ValueNode & target_node, slw::span<ValueNode> const & nodes)
{
    const i64 node_idx = (i64)std::distance(nodes.data(), &node);
    i64 prev_node = target_node.prev;
    target_node.prev = node_idx;
    nodes[prev_node].next = node_idx;
    node.prev = prev_node;
    node.next = (i64)std::distance(nodes.data(), &target_node);
}

// 15297
void advent::runDay20()
{
    sb_log_info("Running advent day 20 trivia ...\n");

    FileHdl file_hdl = vfs::openFileRead("/data/day20.txt");
    sb_assert(file_hdl.isValid());

    FileSize const file_len = vfs::getFileLength(file_hdl);
    MemoryArena const file_buffer = sb_malloc(GHEAP, file_len + 1);
    u8 * const byte_buffer = static_cast<u8 *>(file_buffer.data);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;
    vfs::closeFile(file_hdl);

    slw::string_view file_string_buffer(reinterpret_cast<char const *>(byte_buffer));
    auto tokens_raw = splitString(file_string_buffer, '\n');

    DArray<ValueNode> values;
    values.reserve(tokens_raw.size());
    sb::i64 prev_idx = -1;
    sb::i64 next_idx = 1;
    sb::i64 zero_idx = -1;
     //sb::i64 key_value = 1;
    sb::i64 key_value = 811589153;
    for (auto & token : tokens_raw)
    {
        token = trim(token);

        if (token.empty())
        {
            break;
        }

        ValueNode new_node;

        sb::i64 val = 0;
        std::from_chars(token.data(), token.data() + token.size(), new_node.value);

        i64 new_value = std::abs(new_node.value) * key_value;
        sb_assert(new_value >= std::abs(new_node.value));
        new_node.value = new_node.value * key_value;

        new_node.prev = prev_idx;
        new_node.next = next_idx;

        if (new_node.value == 0)
        {
            sb_assert(zero_idx = -1);
            zero_idx = next_idx - 1;
        }

        values.push_back(new_node);

        ++prev_idx;
        ++next_idx;
    }

    const u64 value_cnt = (u64)values.size();
    values.back().next = 0;
    values.begin()->prev = (i64)values.size() - 1;
    ValueNode const * const zero_node = &values[zero_idx];
    ValueNode * list_begin = values.data();
    ValueNode * list_last = &values.back();

    sb_assert(0 == zero_node->value);

    //dbgPrintList(list_begin, values);

    for (i32 iter_idx = 0; iter_idx != 10; ++iter_idx)
    {
        for (auto & value_node : values)
        {
            sb_assert(&values[list_begin->prev] == list_last);
            sb_assert(&values[list_last->next] == list_begin);

            i64 curr_value = value_node.value;

            if (curr_value > 0)
            {
                ValueNode * node = &value_node;
                unlinkNode(value_node, values);

                if (&value_node == list_begin)
                {
                    list_begin = &values[value_node.next];
                }
                else if (&value_node == list_last)
                {
                    list_last = &values[value_node.prev];
                }

                node = &values.at(node->next);
                --curr_value;
                curr_value = curr_value % ((u32)value_cnt - 1);

                while (curr_value != 0)
                {
                    node = &values.at(node->next);
                    curr_value--;
                }

                linkNodeAfter(value_node, *node, values);
                if (list_last == node)
                {
                    list_begin = &value_node;
                }
            }
            else if (curr_value < 0)
            {
                ValueNode * node = &value_node;
                curr_value = -curr_value;
                unlinkNode(value_node, values);

                if (&value_node == list_begin)
                {
                    list_begin = &values[value_node.next];
                }
                else if (&value_node == list_last)
                {
                    list_last = &values[value_node.prev];
                }

                node = &values.at(node->prev);
                --curr_value;
                curr_value = curr_value % ((u32)value_cnt - 1);

                while (curr_value != 0)
                {
                    node = &values.at(node->prev);
                    curr_value--;
                }

                linkNodeBefore(value_node, *node, values);
                if (list_begin == node)
                {
                    list_last = &value_node;
                }
            }
            // 0: no move
            else
            {
            }

            // dbgPrintList(list_begin, values);
        }
        //dbgPrintList(zero_node, values);
    }

    sb::DArray<i64> final_values;
    final_values.reserve(values.size());

    u64 new_zero_idx = 0xFFFFFFFF;
    auto iter = list_begin;
    u64 idx = 0;
    do
    {
        if (iter->value == 0)
        {
            sb_assert(new_zero_idx == 0xFFFFFFFF);
            new_zero_idx = idx;
        }
        final_values.push_back(iter->value);
        iter = &values[iter->next];
        ++idx;
    } while (iter != list_begin);

    sb_log_info("zero index {}", new_zero_idx);
    // const i64 val1000 = getNthItem(&values[zero_idx], 1000, values);
    // const i64 val2000 = getNthItem(&values[zero_idx], 2000, values);
    // const i64 val3000 = getNthItem(&values[zero_idx], 3000, values);

    const i64 val1000 = final_values[(new_zero_idx + 1000) % final_values.size()];
    const i64 val2000 = final_values[(new_zero_idx + 2000) % final_values.size()];
    const i64 val3000 = final_values[(new_zero_idx + 3000) % final_values.size()];

    sb_log_info("{} {} {}", val1000, val2000, val3000);
    sb_log_info("result {}", val1000 + val2000 + val3000);
    sb_log_info("Job done");

    sb_free(GHEAP, byte_buffer);
}
