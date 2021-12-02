#pragma once
#include <cstdint>

namespace urs::offsets
{
    inline std::uintptr_t fire_server_address = 0;

    constexpr std::uint16_t parent = 0x34;
    constexpr std::uint16_t name = 0x28;
    constexpr std::uint16_t string_length = 0x14;

    inline void init_offsets()
    {
        const auto call_fire_server_address = urs::utils::pattern_scan("\xE8\x00\x00\x00\x00\x8D\x4D\xA4\xE8\x00\x00\x00\x00\xC7\x45\x00\x00\x00\x00\x00\x8B\x75\xEC", "x????xxxx????xx?????xxx").back();

        urs::offsets::fire_server_address = *reinterpret_cast<std::uintptr_t*>(call_fire_server_address + 1) + call_fire_server_address + 5;
    }
}