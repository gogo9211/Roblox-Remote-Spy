#pragma once
#include <cstdint>

namespace urs::arg_handlers
{
    void read_string(int index, std::uintptr_t arg);
    void read_double(int index, std::uintptr_t arg);
    void read_bool(int index, std::uintptr_t arg);
    void read_vector2(int index, std::uintptr_t arg);
    void read_vector3(int index, std::uintptr_t arg);
    void read_cframe(int index, std::uintptr_t arg);
    void read_color3(int index, std::uintptr_t arg);
    void read_instance(int index, std::uintptr_t arg);
    void read_ray(int index, std::uintptr_t arg);
}