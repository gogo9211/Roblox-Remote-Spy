#include <iostream>
#include "arg_handlers.hpp"

#include "../utils.hpp"

void urs::arg_handlers::read_string(int index, std::uintptr_t arg) {
    const auto str = urs::utils::read_string(arg + 0x8);

    std::printf("Arg Value %i: %s\n", index, str.c_str());
}

void urs::arg_handlers::read_double(int index, std::uintptr_t arg) {
    const auto number = *reinterpret_cast<double*>(arg + 0x8);

    std::printf("Arg Value %i: %f\n", index, number);
}

void urs::arg_handlers::read_bool(int index, std::uintptr_t arg) {
    const auto state = *reinterpret_cast<std::uint8_t*>(arg + 0x8);

    std::printf("Arg Value %i: %s\n", index, state ? "true" : "false");
}

void urs::arg_handlers::read_vector2(int index, std::uintptr_t arg) {
    const auto x = *reinterpret_cast<float*>(arg + 0x8);
    const auto y = *reinterpret_cast<float*>(arg + 0xC);

    std::printf("Arg Value %i: {%f, %f}\n", index, x, y);
}

void urs::arg_handlers::read_vector3(int index, std::uintptr_t arg) {
    const auto x = *reinterpret_cast<float*>(arg + 0x8);
    const auto y = *reinterpret_cast<float*>(arg + 0xC);
    const auto z = *reinterpret_cast<float*>(arg + 0x10);

    std::printf("Arg Value %i: {%f, %f, %f}\n", index, x, y, z);
}

void urs::arg_handlers::read_cframe(int index, std::uintptr_t arg) {
    const auto m11 = *reinterpret_cast<float*>(arg + 0x8);
    const auto m12 = *reinterpret_cast<float*>(arg + 0xC);
    const auto m13 = *reinterpret_cast<float*>(arg + 0x10);
    const auto m21 = *reinterpret_cast<float*>(arg + 0x14);
    const auto m22 = *reinterpret_cast<float*>(arg + 0x18);
    const auto m23 = *reinterpret_cast<float*>(arg + 0x1C);
    const auto m31 = *reinterpret_cast<float*>(arg + 0x20);
    const auto m32 = *reinterpret_cast<float*>(arg + 0x24);
    const auto m33 = *reinterpret_cast<float*>(arg + 0x28);
    const auto x = *reinterpret_cast<float*>(arg + 0x2C);
    const auto y = *reinterpret_cast<float*>(arg + 0x30);
    const auto z = *reinterpret_cast<float*>(arg + 0x34);

    std::printf("Arg Value %i: {%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f}\n", index, x, y, z, m11, m12, m13, m21, m22, m23, m31, m32, m33);
}

void urs::arg_handlers::read_color3(int index, std::uintptr_t arg) {
    const auto r = *reinterpret_cast<float*>(arg + 0x8);
    const auto g = *reinterpret_cast<float*>(arg + 0xC);
    const auto b = *reinterpret_cast<float*>(arg + 0x10);

    std::printf("Arg Value %i: {%f, %f, %f}\n", index, r, g, b);
}

void urs::arg_handlers::read_instance(int index, std::uintptr_t arg) {
    const auto instance = *reinterpret_cast<std::uintptr_t*>(arg + 0x8);
    const auto name = utils::get_instance_path(instance);

    std::printf("Arg Value %i: %s\n", index, name.c_str());
}

void urs::arg_handlers::read_ray(int index, std::uintptr_t arg) {
    const auto origin_x = *reinterpret_cast<float*>(arg + 0x8);
    const auto origin_y = *reinterpret_cast<float*>(arg + 0xC);
    const auto origin_z = *reinterpret_cast<float*>(arg + 0x10);

    const auto dir_x = *reinterpret_cast<float*>(arg + 0x14);
    const auto dir_y = *reinterpret_cast<float*>(arg + 0x18);
    const auto dir_z = *reinterpret_cast<float*>(arg + 0x1C);

    std::printf("Arg Value %i: {%f, %f, %f}, {%f, %f, %f}\n", index, origin_x, origin_y, origin_z, dir_x, dir_y, dir_z);
}