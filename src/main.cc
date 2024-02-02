#include <cstdint>
#include <cstring>

#include <memory>
#include <string>
#include <thread>

#include <stdexcept>
#include <string_view>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <discord_rpc.h>

#include <Windows.h>

#include "const/config.hh"

#include "discord/discord_rpc.hh"

#include "utils/process_monitor.hh"
#include "utils/system_tray.hh"

#include "gui/settings.hh"

auto init_system_tray() -> void
{
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = flrpc::system_tray::window_proc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "FLRPC";

    if (!RegisterClass(&wc))
    {
        SPDLOG_ERROR("Failed to register window class. Error code: {}", GetLastError());
        return;
    }

    HWND hwnd = CreateWindowEx(0, "FLRPC", "FL-RPC", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);

    if (!hwnd)
    {
        SPDLOG_ERROR("Failed to create window. Error code: {}", GetLastError());
        return;
    }

    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

#define _tmain main
auto _tmain(int argc, char const *argv[]) -> int
{
    std::shared_ptr<spdlog::logger> console = spdlog::stdout_color_mt("console");
    
    if (!console)
        throw std::runtime_error("unlogical shit just happened, go fuck ur siblings");

    console->set_level(spdlog::level::debug);

    spdlog::set_default_logger(console);

    spdlog::set_pattern("[%H:%M:%S.%f] [%^%l%$] [%s:%#] %v");

    SPDLOG_INFO("FL-RPC @@ Discord Rich Presence for FL Studio");
    SPDLOG_INFO("copyright (C) 2024 SFINXV. All rights reserved");

    flrpc::system_tray::add_context_menu("Force shut down my PC :D", MF_STRING, [](HWND hwnd) -> void {
        auto init_sys_shutdown = [hwnd]() -> void
        {
            if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_MINOR_UPGRADE | SHTDN_REASON_FLAG_PLANNED))
            {
                DWORD error = GetLastError();
                MessageBox(hwnd, "i tried my best, but i got declined :(", "it's sad to know if u didn't love me", MB_OK | MB_ICONERROR);
            }
        };

        HANDLE token;
        if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token))
        {
            TOKEN_PRIVILEGES tkp;
            LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

            tkp.PrivilegeCount = 1;
            tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

            if (AdjustTokenPrivileges(token, FALSE, &tkp, 0, NULL, NULL))
            {
                MessageBox(hwnd, "im gonna hack ur pc :v, say bye", "hi nigga", MB_OK | MB_ICONWARNING);
                init_sys_shutdown();
            }
        }
    });

    flrpc::system_tray::add_separator();

    flrpc::system_tray::add_context_menu("Exit", MF_STRING, [](HWND hwnd) -> void {
        PostQuitMessage(0);
        exit(0);
    });

    // gui test
    /*flrpc::gui::settings gui_test;
    std::thread([&gui_test]() -> void { gui_test.show(); }).detach();*/

    std::thread(init_system_tray).detach();

    flrpc::discord_rpc rpc(flrpc::config::clientid);
    flrpc::process_monitor monitor;

    std::string project_name = "";
    std::string app_name = "";

    /*SPDLOG_INFO("initialization successfull, the window will hide in 5 seconds");

    Sleep(5000);
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);*/

    std::thread(init_system_tray).detach();

    while (true)
    {
        monitor.update_process_info();

        if (monitor.is_running())
        {
            std::string title = monitor.get_window_title();

            std::size_t pos = title.rfind(" - ");

            if (pos != std::string::npos)
            {
                project_name = title.substr(0, pos);
                app_name = title.substr(pos + 3);

                rpc.update_presence(project_name);
            }
            else
            {
                app_name = title;
                rpc.update_presence("Not opening any projects");
            }
        }
        else
        {
            SPDLOG_INFO("presence clear called");
            rpc.clear_presence();
        }

        Sleep(5000);
    }
    
    return 0;
}