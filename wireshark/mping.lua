-- SPDX-FileCopyrightText: Copyright (C) 2024-2025 Marek Küthe <m.k@mk16.de>
--
-- SPDX-License-Identifier: GPL-3.0-or-later

-- WireShark plugin directories: Help -> About - Folders -> {Personal,Global} Lua Plugins
-- Requires Lua 5.4

--[[
MPing packet =
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |  Ver  |T|L| 0 |                                               |
    |                                                               |
    |                     Source Host                               |
    |                                                               |
    |               |                                               |
    |                                                               |
    |                     Destination Host                          |
    |                                                               |
    |               | Seq   | PID   | Seconds       | Microseconds  |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

    Ver = VERSION = "2.0\0"
    T = TYPE
    +----------+-------+
    | T = TYPE | VALUE |
    +----------+-------+
    | SENDER   | "s"   |
    | RECEIVER | "r"   |
    +----------+-------+
    L = TTL
    Source Host = Host
    Destination Host = Host
    Seq = Sequence number
    PID = PID or Random Number
    Seconds = Seconds (Monotonic clock)
    Microseconds = Microseconds (Monotonic clock)

    IPv6 Host =
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |10 | P |   0   | IPv6 Address                  | 0             |
    |                            0                                  |
    |                            0                                  |
    |                            0                                  |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    10 = 10 (little endian)
    P = Port

    IPv4 Host =
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    | 2 | P | IPv4  |                   0                           |
    |                            0                                  |
    |                            0                                  |
    |                            0                                  |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    2 = 2 (little endian)

    otherwise bigendian
]]

local mping_plugin_info = {
    version = "1.0.0",
    author = "Marek Küthe",
    repository = "https://codeberg.org/mark22k/mping-sender",
}

local mping_protocol = Proto("mping", "Multicast Ping Protocol")

local MPING_VERSION = "2.0"

local f_type_lookup = {
    [115] = "Sender",
    [114] = "Receiver",
}

local f_host_family_lookup = {
    [2] = "AF_INET",
    [10] = "AF_INET6",
}

local f_version = ProtoField.stringz("mping.version", "Version", base.ASCII)
local f_type = ProtoField.char("mping.type", "Type", base.NONE, f_type_lookup)
local f_ttl = ProtoField.uint8("mping.ttl", "TTL", base.DEC)

local f_source_host_family =
    ProtoField.uint16("mping.src.family", "Source Address Family", base.DEC, f_host_family_lookup)
local f_source_host_ipv4 = ProtoField.ipv4("mping.src.ipv4", "Source IPv4")
local f_source_host_ipv6 = ProtoField.ipv6("mping.src.ipv6", "Source IPv6")
local f_source_host_port = ProtoField.uint16("mping.src.port", "Source Port", base.DEC)

local f_destination_host_family =
    ProtoField.uint16("mping.dst.type", "Destination Address Family", base.DEC, f_host_family_lookup)
local f_destination_host_ipv4 = ProtoField.ipv4("mping.dst.ipv4", "Source IPv4")
local f_destination_host_ipv6 = ProtoField.ipv6("mping.dst.ipv6", "Source IPv6")
local f_destination_host_port = ProtoField.uint16("mping.dst.port", "Destination Port", base.DEC)

local f_sequence_number = ProtoField.uint16("mping.seq_no", "Sequence number", base.DEC)
local f_pid = ProtoField.uint16("mping.pid", "PID", base.DEC)

local f_seconds = ProtoField.uint64("mping.timestamp.seconds", "Seconds", base.DEC)
local f_microseconds = ProtoField.uint64("mping.timestamp.microseconds", "Microseconds", base.DEC)

local e_depracated_version =
    ProtoExpert.new("mping.depracated_version", "Depracated version", expert.group.DEPRECATED, expert.severity.NOTE)
local e_invalid_type =
    ProtoExpert.new("mping.invalid_type", "Invalid Type", expert.group.MALFORMED, expert.severity.ERROR)

local e_invalid_source_host_family = ProtoExpert.new(
    "mping.src.invalid_family",
    "Invalid Source Address Family",
    expert.group.MALFORMED,
    expert.severity.ERROR
)
local e_mismatch_source_host_address = ProtoExpert.new(
    "mping.src.mismatch_addr",
    "Source address does not match the one in the upper header.",
    expert.group.PROTOCOL,
    expert.severity.WARN
)
local e_mismatch_source_host_port = ProtoExpert.new(
    "mping.src.mismatch_port",
    "Source Port does not match the one in the upper header.",
    expert.group.PROTOCOL,
    expert.severity.WARN
)

local e_invalid_destination_host_family = ProtoExpert.new(
    "mping.dst.invalid_family",
    "Invalid Destination Address Family",
    expert.group.MALFORMED,
    expert.severity.ERROR
)
local e_mismatch_destination_host_address = ProtoExpert.new(
    "mping.dst.mismatch_addr",
    "Destination address does not match the one in the upper header.",
    expert.group.PROTOCOL,
    expert.severity.WARN
)
local e_mismatch_destination_host_port = ProtoExpert.new(
    "mping.dst.mismatch_port",
    "Destination Port does not match the one in the upper header.",
    expert.group.PROTOCOL,
    expert.severity.WARN
)

mping_protocol.fields = {
    f_version,
    f_type,
    f_ttl,

    f_source_host_family,
    f_source_host_ipv4,
    f_source_host_ipv6,
    f_source_host_port,
    f_destination_host_family,
    f_destination_host_ipv4,
    f_destination_host_ipv6,
    f_destination_host_port,

    f_sequence_number,
    f_pid,

    f_seconds,
    f_microseconds,
}

mping_protocol.experts = {
    e_depracated_version,
    e_invalid_type,

    e_invalid_source_host_family,
    e_mismatch_source_host_address,
    e_mismatch_source_host_port,
    e_invalid_destination_host_family,
    e_mismatch_destination_host_address,
    e_mismatch_destination_host_port,
}

function mping_protocol.dissector(buffer, pinfo, tree)
    local max_uint64 = UInt64.new(math.maxinteger)

    if buffer:len() < 288 then
        return
    end

    pinfo.cols.protocol = mping_protocol.name

    local subtree = tree:add(mping_protocol, buffer(), mping_protocol.description)

    local version = buffer(0, 4):stringz()
    subtree:add(f_version, buffer(0, 4), version)

    if version ~= MPING_VERSION then
        return
    end

    local type = buffer(4, 1):uint()
    subtree:add(f_type, buffer(4, 1), type)
    if type ~= 115 and type ~= 114 then
        subtree:add_proto_expert_info(e_depracated_version)
    end
    subtree:add(f_ttl, buffer(5, 1))

    local source_host_tree = subtree:add(buffer(8, 128), "Source Host")
    local source_host_family = buffer(8, 2):le_uint()
    source_host_tree:add_le(f_source_host_family, buffer(8, 2), source_host_family)

    if source_host_family == 10 or source_host_family == 2 then
        local source_host_port = buffer(10, 2):uint()
        source_host_tree:add(f_source_host_port, buffer(10, 2), source_host_port)

        if source_host_port ~= pinfo.src_port then
            source_host_tree:add_proto_expert_info(e_mismatch_source_host_port)
        end

        if source_host_family == 10 then -- AF_INET6
            local source_host_ipv6 = buffer(16, 16):ipv6()
            source_host_tree:add(f_source_host_ipv6, buffer(16, 16), source_host_ipv6)

            if source_host_ipv6 ~= pinfo.src then
                source_host_tree:add_proto_expert_info(e_mismatch_source_host_address)
            end
        elseif source_host_family == 2 then -- AF_INET
            local source_host_ipv4 = buffer(12, 4):ipv4()
            source_host_tree:add(f_source_host_ipv4, buffer(12, 4), source_host_ipv4)

            if source_host_ipv4 ~= pinfo.src then
                source_host_tree:add_proto_expert_info(e_mismatch_source_host_address)
            end
        end
    else
        source_host_tree:add_proto_expert_info(e_invalid_source_host_family)
    end

    local destination_host_tree = subtree:add(buffer(136, 128), "Destination Host")
    local destination_host_family = buffer(136, 2):le_uint()
    destination_host_tree:add_le(f_destination_host_family, buffer(136, 2), destination_host_family)

    if destination_host_family == 10 or destination_host_family == 2 then
        local destination_host_port = buffer(138, 2):uint()
        destination_host_tree:add(f_destination_host_port, buffer(138, 2), destination_host_port)

        if destination_host_port ~= pinfo.dst_port then
            destination_host_tree:add_proto_expert_info(e_mismatch_destination_host_port)
        end

        if destination_host_family == 10 then -- AF_INET6
            local destination_host_ipv6 = buffer(144, 16):ipv6()
            destination_host_tree:add(f_destination_host_ipv6, buffer(144, 16), destination_host_ipv6)

            if destination_host_ipv6 ~= pinfo.dst then
                destination_host_tree:add_proto_expert_info(e_mismatch_destination_host_address)
            end
        elseif destination_host_family == 2 then -- AF_INET
            local destination_host_ipv4 = buffer(140, 4):ipv4()
            destination_host_tree:add(f_destination_host_ipv4, buffer(140, 4), destination_host_ipv4)

            if destination_host_ipv4 ~= pinfo.dst then
                destination_host_tree:add_proto_expert_info(e_mismatch_destination_host_address)
            end
        end
    else
        destination_host_tree:add_proto_expert_info(e_invalid_destination_host_family)
    end

    subtree:add(f_sequence_number, buffer(264, 4))
    subtree:add(f_pid, buffer(268, 4))

    local timestamp_tree = subtree:add(buffer(272, 16), "Timestamp")

    local seconds = buffer(272, 8):uint64()
    timestamp_tree:add(f_seconds, buffer(272, 8), seconds)

    local microseconds = buffer(280, 8):uint64()
    timestamp_tree:add(f_microseconds, buffer(280, 8), microseconds)

    if seconds < max_uint64 and microseconds < max_uint64 then -- check if :tonumber() is safe to use
        timestamp_tree:add(
            "[corresponds to absolute time: "
                .. format_time(seconds:tonumber())
                .. ", "
                .. math.tointeger(microseconds:tonumber())
                .. " microseconds]"
        )
    end
end

set_plugin_info(mping_plugin_info)

local udp_port = DissectorTable.get("udp.port")
udp_port:add(4321, mping_protocol)

udp_port:add_for_decode_as(mping_protocol)
