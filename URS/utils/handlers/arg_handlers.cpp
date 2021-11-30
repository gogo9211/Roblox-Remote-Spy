#include "arg_handlers.hpp"

#include <iostream>

#include "../utils.hpp"

void urs::arg_handlers::read_string(int index, std::uintptr_t arg) {
    const auto str = urs::utils::read_string(arg + 0x8);

    std::printf("Arg Value %i: %s\n", index, str.c_str());
}

void urs::arg_handlers::read_double(int index, std::uintptr_t arg) {
    const auto number = *reinterpret_cast<double*>(arg + 0x8);

    std::printf("Arg Value %i: %f\n", index, number);
}

void urs::arg_handlers::read_vector3(int index, std::uintptr_t arg) {
    const auto x = *reinterpret_cast<float*>(arg + 0x8);
    const auto y = *reinterpret_cast<float*>(arg + 0xC);
    const auto z = *reinterpret_cast<float*>(arg + 0x10);

    std::printf("Arg Value %i: {%f, %f, %f}\n", index, x, y, z);
}