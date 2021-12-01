#include "arg_handlers.hpp"

#include <iostream>
#include <array>

#include "../utils.hpp"

enum class arg_type : std::uint16_t
{
    t_string,
    t_float,
    t_int,
    t_double,
    t_bool
};

struct type_data
{
    const char* name;
    std::uint32_t arg_amount;
    arg_type type;
};

auto roblox_types = std::to_array<type_data>
({
    {"Vector3", 3, arg_type::t_float},
    {"Vector2", 2, arg_type::t_float},
    {"CoordinateFrame", 12, arg_type::t_float},
    {"string", 1, arg_type::t_string},
    {"double", 1, arg_type::t_double},
    {"bool", 1, arg_type::t_bool},
    {"Color3", 3, arg_type::t_float},
    {"Ray", 6, arg_type::t_float}
});

struct read_data
{
    std::uint8_t data[0x30];
};

using get_type_t = void(*)(std::uintptr_t type, read_data& out);

void urs::arg_handlers::read_arg(std::uint32_t index, std::uintptr_t arg, const char* const arg_type_name)
{
    const auto get_value = reinterpret_cast<get_type_t>(*reinterpret_cast<std::uintptr_t*>(*reinterpret_cast<std::uintptr_t*>(arg + 4)));

    read_data data{ };

    get_value(arg + 8, data);

    const auto element_p = std::find_if(std::begin(roblox_types), std::end(roblox_types), [&](type_data const& type) { return std::strcmp(type.name, arg_type_name) == 0; });

    if (element_p == std::end(roblox_types))
    {
        std::printf("Unsupported Type\n");

        return;
    }

    std::printf("Arg Value %i: { ", index);

    const auto& element = element_p[0];

    for (auto i = 0u; i < element.arg_amount; ++i)
    {
        const auto spacing = i == element.arg_amount - 1 ? "" : ", ";

        if (element.type == arg_type::t_float)
        {
            const auto arg = reinterpret_cast<std::uintptr_t>(data.data) + i * sizeof(float);

            std::printf("%f%s", *reinterpret_cast<float*>(arg), spacing);
        }
        else if (element.type == arg_type::t_string)
        {
            const auto arg = reinterpret_cast<std::uintptr_t>(data.data);

            std::printf("%s%s", urs::utils::read_string(arg).c_str(), spacing);
        }
        else if (element.type == arg_type::t_int)
        {
            const auto arg = reinterpret_cast<std::uintptr_t>(data.data) + i * sizeof(std::uint32_t);

            std::printf("%i%s", *reinterpret_cast<std::uint32_t*>(arg), spacing);
        }
        else if (element.type == arg_type::t_bool)
        {
            const auto arg = reinterpret_cast<std::uintptr_t>(data.data) + i * sizeof(std::uint8_t);

            std::printf("%s%s", *reinterpret_cast<std::uint8_t*>(arg) ? "true" : "false", spacing);
        }
        else if (element.type == arg_type::t_double)
        {
            const auto arg = reinterpret_cast<std::uintptr_t>(data.data) + i * sizeof(double);

            std::printf("%f%s", *reinterpret_cast<double*>(arg), spacing);
        }
    }

    std::printf(" }\n");
}