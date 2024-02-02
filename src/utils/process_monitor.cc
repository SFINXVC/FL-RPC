#include "process_monitor.hh"

#include <Windows.h>
#include <TlHelp32.h>
#include <handleapi.h>
#include <minwindef.h>
#include <string>
#include <stringapiset.h>
#include <winnls.h>
#include <winuser.h>

namespace flrpc 
{
    auto process_monitor::update_process_info() -> void
    {
        DWORD id = get_process_id();

        if (id != 0)
            m_title = get_main_window_title(id);
    }
    
    auto process_monitor::get_window_title() const -> std::string
    {
        return m_title;
    }
    
    auto process_monitor::is_running() const -> bool
    {
        return get_process_id() != 0;
    }
    
    auto process_monitor::get_process_id() const -> DWORD
    {
        HANDLE process_snap;
        PROCESSENTRY32 pe32;

        process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (process_snap == INVALID_HANDLE_VALUE)
            return 0;

        pe32.dwSize = sizeof(PROCESSENTRY32);
        
        if (!Process32First(process_snap, &pe32))
        {
            CloseHandle(process_snap);
            return 0;
        }

        DWORD id = 0;
        while (Process32Next(process_snap, &pe32))
        {
            if (strcmp(pe32.szExeFile, "FL.exe") == 0 || strcmp(pe32.szExeFile, "FL64.exe") == 0)
            {
                id = pe32.th32ProcessID;
                break;
            }
        }

        CloseHandle(process_snap);

        return id;
    }
    
    auto process_monitor::get_main_window_title(DWORD id) const -> std::string
    {
        CHAR title[MAX_PATH] = "";
        HWND hwnd = NULL;

        while ((hwnd = FindWindowEx(NULL, hwnd, NULL, NULL)) != NULL)
        {
            DWORD pid;
            GetWindowThreadProcessId(hwnd, &pid);

            if (pid == id && GetParent(hwnd) == NULL)
            {
                GetWindowText(hwnd, title, MAX_PATH);
                std::string str_title(title);

                if (str_title.find("FL Studio") != std::string::npos)
                    break;
            }
        }

        return title;
    }
}