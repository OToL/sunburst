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

#include <charconv>
#include <algorithm>
#include <variant>
#include <memory>
#include <stack>

using namespace sb;

struct Packet
{
    DArray<std::variant<std::unique_ptr<Packet>, int>> values;
};

using PacketValues = std::span<std::variant<std::unique_ptr<Packet>, int> const>;

void parsePacket(slw::string_view str, Packet & packet)
{
    std::stack<Packet *> packet_stack;
    packet_stack.push(&packet);

    size_t idx = 1;
    while (!packet_stack.empty())
    {
        if ('[' == str[idx])
        {
            auto new_packet = std::make_unique<Packet>();
            auto prev_top = packet_stack.top();
            packet_stack.push(new_packet.get());
            prev_top->values.push_back(std::move(new_packet));
            idx++;
        }
        else if (',' == str[idx])
        {
            idx++;
        }
        else if (']' == str[idx])
        {
            packet_stack.pop();
            idx++;
        }
        else
        {
            int val = 0;
            auto [ptr, ec] = std::from_chars(str.data() + idx, str.data() + str.size(), val);
            sb_assert(ec == std::errc());
            idx = std::distance(str.data(), ptr);
            packet_stack.top()->values.push_back(val);
        }
    }
}

enum class PacketItemType
{
    INT,
    PACKET
};

enum class ProcRes
{
    ORDERED,
    NOT_ORDERED,
    UNKNOWN
};

PacketItemType getPacketItemType(std::variant<std::unique_ptr<Packet>, int> const & val)
{
    return std::holds_alternative<int>(val) ? PacketItemType::INT : PacketItemType::PACKET;
}

ProcRes comparePackets(PacketValues const & lval, PacketValues const & rval)
{
    size_t idx = 0;
    ProcRes cmp_res = ProcRes::UNKNOWN;
    while ((idx < lval.size()) && (idx < rval.size()))
    {
        auto const & curr_lval = lval[idx];
        auto const & curr_rval = rval[idx];

        PacketItemType const lval_type = getPacketItemType(curr_lval);
        PacketItemType const rval_type = getPacketItemType(curr_rval);

        if (lval_type == rval_type)
        {
            if (lval_type == PacketItemType::INT)
            {
                auto const lint = std::get<int>(curr_lval);
                auto const rint = std::get<int>(curr_rval);

                if (lint < rint)
                {
                    cmp_res = ProcRes::ORDERED;
                    break;
                }
                else if (lint > rint)
                {
                    cmp_res = ProcRes::NOT_ORDERED;
                    break;
                }
                else
                {
                    // continue comparing
                }
            }
            else
            {
                cmp_res = comparePackets(std::get<std::unique_ptr<Packet>>(curr_lval)->values, std::get<std::unique_ptr<Packet>>(curr_rval)->values);
                if (cmp_res != ProcRes::UNKNOWN)
                {
                    break;
                }
            }
        }
        else
        {
            if (lval_type == PacketItemType::INT)
            {
                cmp_res = comparePackets(lval.subspan(idx, 1), std::get<std::unique_ptr<Packet>>(curr_rval)->values);
            }
            else
            {
                cmp_res = comparePackets(std::get<std::unique_ptr<Packet>>(curr_lval)->values, rval.subspan(idx, 1));
            }

            if (cmp_res != ProcRes::UNKNOWN)
            {
                break;
            }
        }

        ++idx;
    }

    if (cmp_res == ProcRes::UNKNOWN)
    {
        if ((idx != lval.size()) || (idx != rval.size()))
        {
            sb_assert((idx == lval.size()) || (idx == rval.size()));
            cmp_res = idx == lval.size() ? ProcRes::ORDERED : ProcRes::NOT_ORDERED;
        }
    }

    return cmp_res;
}

// Solution
//   6272
//   22288
// @todo: use visit
// @todo: create a specific method to compare Packets on top of values
// @todo: create comparison and order operators on Packet
// @todo: do not use unique_ptr
// @todo: implement a debug print for Packet e.g. compatible with string format i.e. sb_log_information("{}", my_packet)
void advent::runDay13()
{
    sb_log_info("Running advent day 13 trivia ...\n");

    FileHdl file_hdl = vfs::openFileRead("/data/day13.txt");
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

    size_t total_ordered = 0;
    size_t pair_idx = 1;
    DArray<Packet> packets;

    for (size_t idx = 0; idx <= tokens.size(); idx += 3, ++pair_idx)
    {
        Packet packet1, packet2;
        parsePacket(tokens[idx], packet1);
        parsePacket(tokens[idx + 1], packet2);

        auto const res = comparePackets(packet1.values, packet2.values);
        if ((ProcRes::UNKNOWN == res) || (ProcRes::ORDERED == res))
        {
            total_ordered += pair_idx;
        }

        packets.emplace_back(std::move(packet1));
        packets.emplace_back(std::move(packet2));
    }

    Packet extra_packet2;
    Packet extra_packet6;
    parsePacket("[[2]]", extra_packet2);
    parsePacket("[[6]]", extra_packet6);

    packets.emplace_back(std::move(extra_packet2));
    packets.emplace_back(std::move(extra_packet6));

    std::sort(begin(packets), end(packets), [](auto const &lval, auto const &rval)
            {
                auto res = comparePackets(lval.values, rval.values);
                return ((ProcRes::UNKNOWN == res) || (ProcRes::ORDERED == res));
            });

    Packet extra_packet2_bis;
    Packet extra_packet6_bis;
    parsePacket("[[2]]", extra_packet2_bis);
    parsePacket("[[6]]", extra_packet6_bis);

    auto const sep2_it = std::find_if(begin(packets), end(packets), [rval=&extra_packet2_bis](auto const & lval)
            {
                auto res = comparePackets(lval.values, rval->values);
                return (ProcRes::UNKNOWN == res);
            });
    sb_assert(sep2_it != end(packets));

    auto const sep6_it = std::find_if(begin(packets), end(packets), [rval=&extra_packet6_bis](auto const & lval)
            {
                auto res = comparePackets(lval.values, rval->values);
                return (ProcRes::UNKNOWN == res);
            });
    sb_assert(sep6_it != end(packets));

    auto const seps_dist = (std::distance(begin(packets), sep2_it) + 1) * (std::distance(begin(packets), sep6_it) + 1);

    sb_log_info("Total ordered {}", total_ordered);
    sb_log_info("Seperators prod {}", seps_dist);

    sb_free(GHEAP, byte_buffer);

    vfs::readFile(file_hdl, slw::span<u8>(byte_buffer, file_len));
    byte_buffer[file_len] = 0;
    vfs::closeFile(file_hdl);
}
