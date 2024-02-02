#include "system_tray.hh"

#include <codecvt>
#include <locale>

#include <algorithm>

#include <strsafe.h>

namespace flrpc
{
	auto system_tray::interface__initialize(HWND hwnd) -> void
	{
		m_nid.cbSize = sizeof(NOTIFYICONDATA);
		m_nid.hWnd = hwnd;
		m_nid.uID = 1;
		m_nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
		m_nid.uCallbackMessage = WM_USER + 1;
		m_nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		StringCchCopy(m_nid.szTip, ARRAYSIZE(m_nid.szTip), "FL-RPC");
	}

	auto system_tray::interface__show_menu(HWND hwnd) -> void
	{
		HMENU menu = CreatePopupMenu();

		std::uint32_t index = 0;
		for (const auto& i : m_menu)
		{
			AppendMenu(menu, i.m_type, static_cast<UINT_PTR>(index), i.m_name.data());
			index++;
		}

		POINT point;
		GetCursorPos(&point);

		SetForegroundWindow(hwnd);
		TrackPopupMenu(menu, TPM_RIGHTBUTTON, point.x, point.y, 0, hwnd, NULL);
		PostMessage(hwnd, WM_NULL, 0, 0);

		DestroyMenu(menu);
	}

	auto system_tray::interface__add_context_menu(std::string_view name, int type, callback_t fn) -> void
	{
		contextmenu_t ctx;
		ctx.m_name = name.data();
		ctx.m_type = type;
		ctx.m_callback = fn;

		m_menu.push_back(ctx);
	}

	auto system_tray::interface__add_separator() -> void
	{
		interface__add_context_menu("", MF_SEPARATOR, [](HWND hwnd) -> void {});
	}

	auto system_tray::interface__execute_commands(UINT index, HWND hwnd) -> void
	{
		if (m_menu.size() < index)
			return;

		m_menu[index].m_callback(hwnd);
	}

	auto system_tray::interface__show(bool state) -> void
	{
		Shell_NotifyIcon(NIM_ADD, &m_nid);
	}

	LRESULT CALLBACK system_tray::interface__window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
			case WM_CREATE:
			{
				interface__initialize(hwnd);
				interface__show(true);

				break;
			}
			
			case WM_USER + 1:
			{
				switch (lparam)
				{
					case WM_RBUTTONUP:
					{
						interface__show_menu(hwnd);
						break;
					}
					case WM_LBUTTONUP:
					{
						::ShowWindow(::GetConsoleWindow(), SW_SHOW);
						break;
					}
				}

				break;
			}

			case WM_COMMAND:
			{
				interface__execute_commands(LOWORD(wparam), hwnd);
				break;
			}

			case WM_DESTROY:
			{
				interface__show(false);
				PostQuitMessage(0);

				break;
			}

			default:
				return DefWindowProc(hwnd, msg, wparam, lparam);
		}

		return 0;
	}
}