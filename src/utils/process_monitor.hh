#ifndef __UTILS__PROCESS_MONITOR__HH__
#define __UTILS__PROCESS_MONITOR__HH__

#include <string>
#include <Windows.h>

namespace flrpc
{
    class process_monitor
    {
    public:
        auto update_process_info() -> void;
        auto get_window_title() const -> std::string;
        auto is_running() const -> bool;

    private:
        auto get_process_id() const -> DWORD;
        auto get_main_window_title(DWORD id) const -> std::string;

    private:
        std::string m_title;

    };
}

#endif // __UTILS__PROCESS_MONITOR__HH__