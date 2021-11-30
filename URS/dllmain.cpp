#include <Windows.h>
#include <iostream>
#include <thread>

#include "utils/utils.hpp"
#include "utils/bypasses/memcheck/memcheck.hpp"
#include "utils/offsets/offsets.hpp"
#include "utils/handlers/arg_handlers.hpp"

using fire_server_t = void(__thiscall*)(std::uintptr_t this_ptr, std::uintptr_t args, std::uint8_t unk);
auto fire_server = reinterpret_cast<fire_server_t>(0);

const std::uint32_t ARG_STRUCT_SIZE = 0x48;

void __fastcall fire_server_hook(std::uintptr_t this_ptr, std::uintptr_t edx, std::uintptr_t args, std::uint8_t unk)
{
    std::printf("---START---\n\n");

    std::printf("FireServer Called: %s\n", urs::utils::get_instance_path(this_ptr).c_str());

    if (!args)
    {
        std::printf("\n---END---\n\n");

        return fire_server(this_ptr, args, unk);
    }

    const auto vector_start = *reinterpret_cast<std::uintptr_t*>(args);
    const auto vector_end = *reinterpret_cast<std::uintptr_t*>(args + 4);

    const auto vector_size = vector_end - vector_start;
    const auto number_of_args = vector_size / ARG_STRUCT_SIZE;

    std::printf("Number of Args: %i\n", number_of_args);

    for (auto i = 0u; i < number_of_args; ++i)
    {
        const auto arg = vector_start + (i * ARG_STRUCT_SIZE);

        const auto arg_type = *reinterpret_cast<std::uintptr_t*>(arg);
        const auto arg_type_name = *reinterpret_cast<const char**>(arg_type + 4);
        
        std::printf("\nArg Type %i: %s\n", i, arg_type_name);

        if (std::strcmp(arg_type_name, "string") == 0)
        {
            urs::arg_handlers::read_string(i, arg);
        }
        else if (std::strcmp(arg_type_name, "double") == 0)
        {
            urs::arg_handlers::read_double(i, arg);
        }
        else if (std::strcmp(arg_type_name, "Vector3") == 0)
        {
            urs::arg_handlers::read_vector3(i, arg);
        }
        else
        {
            std::printf("Unsupported Type\n");
        }
    }

    std::printf("\n---END---\n\n");

    return fire_server(this_ptr, args, unk);
}

void d_main()
{
    urs::utils::hook_free_console();

    AllocConsole();
    FILE* file_stream;

    freopen_s(&file_stream, "CONIN$", "r", stdin);
    freopen_s(&file_stream, "CONOUT$", "w", stdout);
    freopen_s(&file_stream, "CONOUT$", "w", stderr);

    SetConsoleTitleA("[URS]");

    std::printf("[URS] Undetected Remote Spy\n\n");

    memcheck::bypass();

    const auto fire_server_address = reinterpret_cast<std::uintptr_t>(GetModuleHandleA(nullptr)) + urs::offsets::fire_server_address;

    fire_server = reinterpret_cast<fire_server_t>(urs::utils::tramp_hook(fire_server_address, reinterpret_cast<std::uintptr_t>(fire_server_hook), 6));

    std::printf("[URS] FireServer Hooked!\n\n");
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        std::thread(d_main).detach();

    return TRUE;
}