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
const std::string BLACKLISTED_PATH = "Game.RobloxReplicatedStorage.IntegrityCheckProcessorKey_LocalizationTableEntryStatisticsSender_LocalizationService";

// is_deep will determine if the vector is a lua table in the args list (true) or if it is the actual arg list vector (false).
void handle_vector(std::uintptr_t args, bool is_deep = false)
{
    const auto vector_start = *reinterpret_cast<std::uintptr_t*>(args);
    const auto vector_end = *reinterpret_cast<std::uintptr_t*>(args + 4);

    const auto vector_size = vector_end - vector_start;
    const auto number_of_args = vector_size / ARG_STRUCT_SIZE;

    if (is_deep)
        std::printf("\n{\n");

    std::printf("Number of Args: %i\n", number_of_args);

    for (auto i = 0u; i < number_of_args; ++i)
    {
        const auto arg = vector_start + (i * ARG_STRUCT_SIZE);

        const auto arg_type = *reinterpret_cast<std::uintptr_t*>(arg);
        const auto arg_type_name = *reinterpret_cast<const char**>(arg_type + 4);

        std::printf("\nArg Type %i: %s\n", i, arg_type_name);

        if (std::strcmp(arg_type_name , "Array") == 0)
        {
            const auto vector = *reinterpret_cast<std::uintptr_t*>(arg + 8);

            handle_vector(vector, true);

            continue;
        }

        urs::arg_handlers::read_arg(i, arg, arg_type_name);
    }

    if (is_deep)
        std::printf("}\n");
}

void __fastcall fire_server_hook(std::uintptr_t this_ptr, std::uintptr_t edx, std::uintptr_t args, std::uint8_t unk)
{
    const auto remote_path = urs::utils::get_instance_path(this_ptr);
    if (remote_path == BLACKLISTED_PATH) return;

    std::printf("---START---\n\n");

    std::printf("FireServer Called: %s\n", remote_path.c_str());

    if (!args)
    {
        std::printf("Number of Args: 0\n");
        std::printf("\n---END---\n\n");

        return fire_server(this_ptr, args, unk);
    }

    handle_vector(args);

    std::printf("\n---END---\n\n");

    return fire_server(this_ptr, args, unk);
}

std::uintptr_t invoker_server_start;
std::uintptr_t invoker_server_end;

//not in the mood of figuring out how roblox push invoke server args so i put ty kind jmp to this stub which will execute the overriden instructions, save registers, then u can do whatevr u want, then we restore and jmp back, no more need to know amount of args etc...
_declspec(naked) void invoke_server_hook()
{
    //u can declare variables here safely, then get the arg u need by doing something like

    /*
    push eax //save eax
    mov eax, esp + 8 //mov arg in eax
    mov locvar, eax, //mov the variable u declared to eax ^^
    pop eax //restore eax
    */

    //when u get the arg here ^^, use it where the printf is, not before or else

    __asm
    {
        push ebx
        mov ebx, esp
        sub esp, 8

        pushad
    }

    std::printf("Invoker Server Called\n");

    __asm
    {
        popad
        jmp invoker_server_end
    }
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

    urs::offsets::init_offsets();
    memcheck::bypass();

    fire_server = reinterpret_cast<fire_server_t>(urs::utils::tramp_hook(urs::offsets::fire_server_address, reinterpret_cast<std::uintptr_t>(fire_server_hook), 6));

    invoker_server_start = urs::offsets::invoke_server_address;
    invoker_server_end = invoker_server_start + 0x6;

    DWORD old_protect{};

    VirtualProtect(reinterpret_cast<void*>(invoker_server_start), 0x6, PAGE_EXECUTE_READWRITE, &old_protect);

    std::memset(reinterpret_cast<void*>(invoker_server_start), 0x90, 0x6);

    *reinterpret_cast<std::uint8_t*>(invoker_server_start) = 0xE9;
    *reinterpret_cast<std::uintptr_t*>(invoker_server_start + 1) = (reinterpret_cast<std::uintptr_t>(invoke_server_hook) - invoker_server_start - 5);

    VirtualProtect(reinterpret_cast<void*>(invoker_server_start), 0x6, old_protect, &old_protect);

    std::printf("[URS] FireServer Hooked!\n");
    std::printf("[URS] InvokeServer Hooked!\n\n");
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        std::thread(d_main).detach();

    return TRUE;
}