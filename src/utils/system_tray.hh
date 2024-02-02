#ifndef __UTILS__SYSTEM_TRAY__HH__
#define __UTILS__SYSTEM_TRAY__HH__

#include <Windows.h>
#include <shellapi.h>

#include <string>
#include <vector>

#include <functional>
#include <string_view>

namespace flrpc
{
	using callback_t = std::function<void(HWND)>;

	struct contextmenu_t
	{
		std::string m_name = "";
		std::int32_t m_type = MF_STRING;
		callback_t m_callback;
	};

	class system_tray
	{
	public:
		static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) { return get().interface__window_proc(hwnd, msg, wparam, lparam); }
		
		static auto add_context_menu(std::string_view name, int type, callback_t fn) -> void { return get().interface__add_context_menu(name, type, fn); }
		static auto add_separator() -> void { return get().interface__add_separator(); }

		static system_tray& get() { static system_tray s; return s; }

	private:
		auto interface__initialize(HWND hwnd) -> void;
		auto interface__show_menu(HWND hwnd) -> void;

		auto interface__add_context_menu(std::string_view name, int type, callback_t fn) -> void;
		auto interface__add_separator() -> void;
		auto interface__execute_commands(UINT index, HWND hwnd) -> void;

		auto interface__show(bool state) -> void;

		LRESULT CALLBACK interface__window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	private:
		NOTIFYICONDATA m_nid;

		std::vector<contextmenu_t> m_menu;
	};
}

#endif // __UTILS__SYSTEM_TRAY__HH__
