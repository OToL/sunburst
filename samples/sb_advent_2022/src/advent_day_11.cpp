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
#include <sb_core/containers/ring_buffer.h>

#include <sb_slw/string_view>

#include <ranges>
#include <charconv>
#include <algorithm>
#include <vector>
#include <xutility>
#include <functional>

using namespace sb;

usize computeLCM(usize a, usize b)
{
    usize lcm = 0;
    if (a > b)
        lcm = a;
    else
        lcm = b;
    while (true)
    {
        if ((lcm % a == 0) && (lcm % b == 0))
        {
            return lcm;
        }
        lcm++;
    }

    // return 0;
}

// Solution
// @todo: study modular arithmetic
void advent::runDay11()
{
    sb_log_info("Running advent day 11 trivia ...\n");

    FileHdl file_hdl = vfs::openFileRead("/data/day11.txt");
    sb_assert(file_hdl.isValid());

    FileSize const file_len = vfs::getFileLength(file_hdl);
    MemoryArena const file_buffer = sb_malloc(GHEAP, file_len + 1);
    u8 * const byte_buffer = static_cast<u8 *>(file_buffer.data);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;

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

    struct Monkey
    {
        Monkey()
            : items(128)
        {
        }

        RingBuffer<usize> items;
        usize op_lval = 0;
        usize op_rval = 0;
        char op = 0;
        usize div_by = 0;
        usize monkey_throw[2];
    };

    constexpr usize OLD_VAL = std::numeric_limits<usize>::max();

    Monkey monkeys[8];
    {
        usize monkey_idx = 0;
        for (auto token : tokens)
        {
            if (token.starts_with("Monkey "))
            {
                std::from_chars(token.data() + strlen("Monkey "), token.data() + token.size(), monkey_idx);
                continue;
            }

            auto & curr_monkey = monkeys[monkey_idx];
            if (token.starts_with("Operation: new = "))
            {
                slw::string_view op_str = token.substr(strlen("Operation: new = "));
                DArray<slw::string_view> operation = splitString(op_str, ' ');
                sb_assert(operation.size() == 3);

                curr_monkey.op = operation[1][0];
                auto param_parser = [](slw::string_view val) -> usize {
                    if (val == "old")
                    {
                        return OLD_VAL;
                    }
                    else
                    {
                        usize ival = 0;
                        std::from_chars(val.data(), val.data() + val.size(), ival);
                        return ival;
                    }
                };
                curr_monkey.op_lval = param_parser(operation[0]);
                curr_monkey.op_rval = param_parser(operation[2]);
            }
            else if (token.starts_with("Starting items: "))
            {
                slw::string_view list_str = token.substr(strlen("Starting items: "));
                DArray<slw::string_view> list = splitString(list_str, ' ');

                for (auto item : list)
                {
                    if (!item.empty())
                    {
                        usize ival = 0;
                        std::from_chars(item.data(), item.data() + item.size(), ival);
                        curr_monkey.items.put(ival);
                    }
                }
            }
            else if (token.starts_with("Test: divisible by "))
            {
                slw::string_view div_str = token.substr(strlen("Test: divisible by "));
                std::from_chars(div_str.data(), div_str.data() + div_str.size(), curr_monkey.div_by);
            }
            else if (token.starts_with("If false: throw to monkey "))
            {
                slw::string_view throw_str = token.substr(strlen("If false: throw to monkey "));
                std::from_chars(throw_str.data(), throw_str.data() + throw_str.size(), curr_monkey.monkey_throw[0]);
            }
            else if (token.starts_with("If true: throw to monkey "))
            {
                slw::string_view throw_str = token.substr(strlen("If true: throw to monkey "));
                std::from_chars(throw_str.data(), throw_str.data() + throw_str.size(), curr_monkey.monkey_throw[1]);
            }
        }
    }
    auto parse_op_val = [](usize val, usize old_value) { return (OLD_VAL == val) ? old_value : val; };

    //{
    //    usize process_cnt[8] = {};
    //    for (usize round = 0; round != 20; ++round)
    //    {
    //        usize curr_monkey_idx = 0;
    //        for (auto & curr_monkey : monkeys)
    //        {
    //            while (!curr_monkey.items.empty())
    //            {
    //                usize inspected_item = curr_monkey.items.pop();
    //                usize const lval = parse_op_val(curr_monkey.op_lval, inspected_item);
    //                usize const rval = parse_op_val(curr_monkey.op_rval, inspected_item);
    //
    //                if ('+' == curr_monkey.op)
    //                {
    //                    inspected_item = lval + rval;
    //                }
    //                else if ('*' == curr_monkey.op)
    //                {
    //                    inspected_item = lval * rval;
    //                }
    //                else
    //                {
    //                    sb_report_error("Unsupported operation");
    //                }
    //
    //                inspected_item /= 3;
    //
    //                usize const next_monkey = curr_monkey.monkey_throw[inspected_item % curr_monkey.div_by == 0];
    //                monkeys[next_monkey].items.put(inspected_item);
    //
    //                process_cnt[curr_monkey_idx]++;
    //            }
    //            curr_monkey_idx++;
    //        }
    //    }
    //
    //    std::sort(std::begin(process_cnt), std::end(process_cnt), std::greater<usize>{});
    //    sb_log_info("Max process {}", process_cnt[0] * process_cnt[1]);
    //}

    {
        usize process_cnt[8] = {};
        for (usize round = 0; round != 1000; ++round)
        {
            usize curr_monkey_idx = 0;
            for (auto & curr_monkey : monkeys)
            {
                while (!curr_monkey.items.empty())
                {
                    usize inspected_item = curr_monkey.items.pop();
                    usize const lval = parse_op_val(curr_monkey.op_lval, inspected_item);
                    usize const rval = parse_op_val(curr_monkey.op_rval, inspected_item);
                    usize const LCM = 9699690ULL; // omputeLCM(lval, rval);

                    if ('+' == curr_monkey.op)
                    {
                        inspected_item = ((lval % LCM) + (rval % LCM)) % LCM;
                    }
                    else if ('*' == curr_monkey.op)
                    {
                        inspected_item = ((lval % LCM) * (rval % LCM)) % LCM;
                    }
                    else
                    {
                        sb_report_error("Unsupported operation");
                    }

                    // inspected_item /= 3;

                    usize const next_monkey = curr_monkey.monkey_throw[(inspected_item % curr_monkey.div_by) == 0];
                    monkeys[next_monkey].items.put(inspected_item);

                    process_cnt[curr_monkey_idx]++;
                }
                curr_monkey_idx++;
            }
        }

        std::sort(std::begin(process_cnt), std::end(process_cnt), std::greater<usize>{});
        sb_log_info("Uber process {}", process_cnt[0] * process_cnt[1]);
    }
    sb_free(GHEAP, byte_buffer);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;
    vfs::closeFile(file_hdl);
}
