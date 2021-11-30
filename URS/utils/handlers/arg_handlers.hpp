#pragma once
#include <cstdint>

namespace urs::arg_handlers
{
    void read_string(int index, std::uintptr_t arg);
    void read_double(int index, std::uintptr_t arg);
    void read_vector3(int index, std::uintptr_t arg);
}