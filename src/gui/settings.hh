#ifndef __GUI__SETTINGS__HH__
#define __GUI__SETTINGS__HH__

#include <Windows.h>

namespace flrpc::gui
{
	class settings
	{
	public:
		settings();
		~settings();

		auto show() -> void;

	private:
		static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	private:
		HWND m_window = nullptr;

		HWND m_label = nullptr;
		HWND m_combobox = nullptr;
		HWND m_linklabel = nullptr;
		HWND m_textbox = nullptr;
		HWND m_button = nullptr;

		HINSTANCE m_instance = NULL;

		WNDCLASSEX m_wc;
	};
}

#endif // !__GUI__SETTINGS__HH__
