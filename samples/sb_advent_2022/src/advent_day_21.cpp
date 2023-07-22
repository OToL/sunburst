#include "advent.h"
#include "sb_core/core.h"
#include "sb_slw/forward_decls"
#include "utility.h"

#include <algorithm>
#include <charconv>
#include <cstdlib>
#include <iterator>
#include <limits>
#include <minwinbase.h>
#include <sb_core/log.h>
#include <sb_core/error.h>
#include <sb_core/string/utility.h>
#include <sb_core/memory/memory.h>
#include <sb_core/memory/global_heap.h>
#include <sb_core/io/virtual_file_system.h>
#include <sb_core/containers/dynamic_array.h>
#include <sb_core/hash/hash_str.h>

#include <sb_slw/string_view>
#include <sb_slw/span>
#include <winnt.h>
#include <unordered_map>

using namespace sb;

#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wunused-but-set-variable"

enum class Op : u32
{
    ADD,
    SUB,
    MUL,
    DIV,
    MATCH,
    NA
};

constexpr char OP_VAL[] = {'+', '-', '*', '/'};

Op getOp(char val)
{
    const auto op_iter = std::find(std::begin(OP_VAL), std::end(OP_VAL), val);
    return static_cast<Op>(std::distance(std::begin(OP_VAL), op_iter));
}

struct MonkeyDesc
{
    i64 value = 0;
    Op op = Op::NA;
    HashStr name;
    HashStr lmonkey;
    HashStr rmonkey;
    slw::string_view lmonkey_str;
    slw::string_view rmonkey_str;
};

constexpr i64 UNKNOWN_VALUE = std::numeric_limits<i64>::max();

struct MonkeyNode
{
    i64 value = 0;
    Op op = Op::NA;
    bool resolved = false;
    u32 lmonkey = 0xFFFFFFFFU;
    u32 rmonkey = 0xFFFFFFFFU;
    i64 lvalue = UNKNOWN_VALUE;
    i64 rvalue = UNKNOWN_VALUE;
};

void resolveRiddle(MonkeyNode & node, slw::span<MonkeyNode> const & nodes)
{
    if (node.op == Op::NA)
    {
        sb_log_info("human value {}", node.value);
        return;
    }

    sb_assert(node.value != UNKNOWN_VALUE);

    MonkeyNode& lmonkey = nodes[node.lmonkey];
    MonkeyNode& rmonkey = nodes[node.rmonkey];

    sb_assert(lmonkey.value == UNKNOWN_VALUE || rmonkey.value == UNKNOWN_VALUE);

    if (lmonkey.value == UNKNOWN_VALUE)
    {
        switch (node.op)
        {
            case Op::ADD:
            {
                lmonkey.value = node.value - rmonkey.value;
                break;
            }
            case Op::SUB:
            {
                lmonkey.value = node.value + rmonkey.value;
                break;
            }
            case Op::MUL:
            {
                lmonkey.value = node.value / rmonkey.value;
                break;
            }
            case Op::DIV:
            {
                lmonkey.value = node.value * rmonkey.value;
                break;
            }
            case Op::MATCH:
            {
                break;
            }
            case Op::NA:
            {
                sb_report_error("unhandled case");
                break;
            }
        }
        resolveRiddle(lmonkey, nodes);
    }
    else
    {
        switch (node.op)
        {
            case Op::ADD:
            {
                rmonkey.value = node.value - lmonkey.value;
                break;
            }
            case Op::SUB:
            {
                rmonkey.value = lmonkey.value - node.value;
                break;
            }
            case Op::MUL:
            {
                rmonkey.value = node.value / lmonkey.value;
                break;
            }
            case Op::DIV:
            {
                rmonkey.value = lmonkey.value / node.value;
                break;
            }
            case Op::MATCH:
            {
                break;
            }
            case Op::NA:
            {
                sb_report_error("unhandled case");
                break;
            }
        }
        resolveRiddle(rmonkey, nodes);
    }
}

i64 parseNode(MonkeyNode& node, slw::span<MonkeyNode> const& nodes)
{
    // Node is a leaf or it has been already processed 
    if ((node.op == Op::NA) || (node.resolved))
    {
        return node.value;
    }

    sb_assert(node.lmonkey != 0xFFFFFFFFU && node.rmonkey != 0xFFFFFFFFU);

    i64 value = UNKNOWN_VALUE;

    i64 lval = parseNode(nodes[node.lmonkey], nodes);
    i64 rval = parseNode(nodes[node.rmonkey], nodes);

    if ((lval != UNKNOWN_VALUE) && (rval != UNKNOWN_VALUE))
    {
        switch (node.op)
        {
            case Op::ADD:
            {
                value = lval + rval;
                break;
            }
            case Op::SUB:
            {
                value = lval - rval;
                break;
            }
            case Op::MUL:
            {
                value = lval * rval;
                break;
            }
            case Op::DIV:
            {
                value = lval / rval;
                break;
            }
            case Op::MATCH:
            {
                break;
            }
            case Op::NA:
            {
                sb_report_error("unhandled case");
                break;
            }
        }
    }

    node.value = value;
    if (node.op == Op::MATCH)
    {
        sb_log_info("found root {} {}", lval, rval);
    }

    node.resolved = true;
    node.rvalue = rval;
    node.lvalue = lval;

    return value;
}

void advent::runDay21()
{
    sb_log_info("Running advent day 21 trivia ...\n");

    FileHdl file_hdl = vfs::openFileRead("/data/day21.txt");
    sb_assert(file_hdl.isValid());

    FileSize const file_len = vfs::getFileLength(file_hdl);
    MemoryArena const file_buffer = sb_malloc(GHEAP, file_len + 1);
    u8 * const byte_buffer = static_cast<u8 *>(file_buffer.data);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;
    vfs::closeFile(file_hdl);

    slw::string_view file_string_buffer(reinterpret_cast<char const *>(byte_buffer));
    auto tokens_raw = splitString(file_string_buffer, '\n');

    std::unordered_map<HashStr::ValueType, u32> monkey_repo;
    DArray<MonkeyDesc> monkey_descs;
    DArray<MonkeyNode> monkey_tree;

    monkey_descs.reserve(tokens_raw.size());
    monkey_repo.reserve(tokens_raw.size());

    for (auto & token : tokens_raw)
    {
        token = trim(token);

        if (token.empty())
        {
            break;
        }

        MonkeyDesc desc;

        auto const end_name_idx = token.find(':');
        sb_assert(end_name_idx != slw::string_view::npos);
        slw::string_view monkey_name = token.substr(0, end_name_idx);

        desc.name = HashStr::make(monkey_name.data(), monkey_name.size());

        token = token.substr(end_name_idx + 2);
        sb::i64 value = 0;
        auto const conv_res = std::from_chars(token.data(), token.data() + token.size(), value);

        if (std::errc::invalid_argument == conv_res.ec)
        {
            auto const lmonkey_idx = token.find(' ');
            sb_assert(lmonkey_idx != slw::string_view::npos);
            slw::string_view lmonkey_name = token.substr(0, lmonkey_idx);
            desc.lmonkey = HashStr::make(lmonkey_name.data(), lmonkey_name.size());

            desc.op = getOp(token[lmonkey_idx + 1]);

            slw::string_view rmonkey_name = token.substr(lmonkey_idx + 3);
            desc.rmonkey = HashStr::make(rmonkey_name.data(), rmonkey_name.size());

            desc.lmonkey_str = lmonkey_name;
            desc.rmonkey_str = rmonkey_name;
        }
        // crying value
        else
        {
            desc.value = value;
        }

        monkey_repo.emplace(desc.name.value, monkey_descs.size());
        monkey_descs.push_back(desc);
    }

    const HashStr ROOT_NAME = HashStr::make("root");
    const HashStr HUMAN_NAME = HashStr::make("humn");

    u32 root_monkey_idx = 0;
    u32 human_monkey_idx = 0;
    monkey_tree.reserve(tokens_raw.size());
    for (auto const & desc : monkey_descs)
    {
        MonkeyNode node;
        node.value = desc.value;
        node.op = desc.op;

        if (ROOT_NAME == desc.name)
        {
            root_monkey_idx = static_cast<u32>(monkey_tree.size());
            node.op = Op::MATCH;
        }
        else if (HUMAN_NAME == desc.name)
        {
            node.value = UNKNOWN_VALUE;
        }

        if (node.op != Op::NA)
        {
            node.lmonkey = monkey_repo[desc.lmonkey.value];
            node.rmonkey = monkey_repo[desc.rmonkey.value];
            sb_assert(node.lmonkey != 0xFFFFFFFFU && node.rmonkey != 0xFFFFFFFFU);
        }

        monkey_tree.push_back(node);
    }

    /*i64 result =*/ parseNode(monkey_tree[root_monkey_idx], monkey_tree);
    //sb_log_info("result: {}", result);

    MonkeyNode& root_monkey = monkey_tree[root_monkey_idx];
    MonkeyNode& lroot_monkey = monkey_tree[root_monkey.lmonkey];
    MonkeyNode& rroot_monkey = monkey_tree[root_monkey.rmonkey];
    const u32 value_monkey_idx = lroot_monkey.value==UNKNOWN_VALUE?root_monkey.rmonkey:root_monkey.lmonkey; 
    const u32 unknown_monkey_idx = lroot_monkey.value==UNKNOWN_VALUE?root_monkey.lmonkey:root_monkey.rmonkey; 

    sb_log_info("target value {}", monkey_tree[value_monkey_idx].value);
    sb_log_info("unknown value {}", monkey_tree[unknown_monkey_idx].value);

    monkey_tree[unknown_monkey_idx].value = monkey_tree[value_monkey_idx].value;
    resolveRiddle(monkey_tree[unknown_monkey_idx], monkey_tree);

    sb_free(GHEAP, byte_buffer);
}
