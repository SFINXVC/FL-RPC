#include "settings.hh"
#include <rpcnsi.h>
#include <winuser.h>

#include <spdlog/spdlog.h>

#define ID_BUTTON 101

namespace flrpc::gui
{
	settings::settings()
		: m_instance(GetModuleHandle(NULL)),
		  m_wc({ sizeof(WNDCLASSEX), CS_DBLCLKS, window_proc, 0, 0, m_instance, LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_WINDOW + 1), NULL, "FLRPC_GUI", LoadIcon(NULL, IDI_APPLICATION) })
	{
		RegisterClassEx(&m_wc);

		m_window = CreateWindowEx(0, "FLRPC_GUI", "FL-RPC Settings", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 300, 200, NULL, NULL, m_instance, NULL);
		m_label = CreateWindow("STATIC", "Choose an option:", WS_CHILD | WS_VISIBLE, 10, 10, 150, 20, m_window, NULL, NULL, NULL);
		m_combobox = CreateWindow("COMBOBOX", "", CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE, 10, 40, 150, 100, m_window, (HMENU)ID_BUTTON, NULL, NULL);
		m_button = CreateWindow("BUTTON", "Click Me", WS_CHILD | WS_VISIBLE, 10, 150, 100, 30, m_window, NULL, NULL, NULL);
		m_linklabel = CreateWindow("STATIC", "click me", WS_CHILD | WS_VISIBLE, 10, 190, 250, 20, m_window, NULL, NULL, NULL);

		// combo-box options
		SendMessage(m_combobox, CB_ADDSTRING, 0, (LPARAM)"Option 1");
		SendMessage(m_combobox, CB_ADDSTRING, 0, (LPARAM)"Option 2");
		SendMessage(m_combobox, CB_ADDSTRING, 0, (LPARAM)"Option 3");
	}

	settings::~settings()
	{
		
	}

	auto settings::show() -> void
	{
		ShowWindow(m_window, SW_SHOW);
		UpdateWindow(m_window);

		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	LRESULT CALLBACK settings::window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
			case WM_CREATE:
				return 0;
				break;

			case WM_COMMAND:
			{
				if (LOWORD(wparam) == ID_BUTTON)
					MessageBox(hwnd, "Button was Clicked!", "Information", MB_OK | MB_ICONINFORMATION);	
				
				break;
			}

			case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;

				break;
			}

			default:
				return DefWindowProc(hwnd, msg, wparam, lparam);
		}

		return 0;
	}
}