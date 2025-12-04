#include <Windows.h>

module Valhalla.Core.Input;

import Valhalla.Core.KeyCodes;
import Valhalla.Core.MouseCodes;


namespace Valhalla::Input
{
	struct Win32InputBackend : IInputBackend
	{
		CursorMode m_CursorMode = CursorMode::Normal;

		HWND m_Window;

		Win32InputBackend(HWND window) : m_Window(window) {}
		
		bool IsKeyPressed(KeyCode keycode) override
		{
			auto state = GetAsyncKeyState(static_cast<int32_t>(keycode));

			return (state & 0x8000);
		}

		bool IsMouseButtonPressed(MouseButton button) override
		{
			auto state = GetAsyncKeyState(static_cast<int32_t>(button));

			return (state & 0x8000);
		}

		std::pair<float_t, float_t> GetMousePosition() override
		{
			POINT p;

			GetCursorPos(&p);

			return { (float_t)p.x, (float_t)p.y };
		}

		float_t GetMouseX() override
		{
			auto [x, y] = GetMousePosition();

			return x;
		}

		float_t GetMouseY() override
		{
			auto [x, y] = GetMousePosition();

			return y;
		}

		void SetCursorMode(CursorMode mode)
		{
			switch (mode)
			{
			case CursorMode::Normal:
				ShowCursor(TRUE);
				ClipCursor(nullptr);
				break;
			case CursorMode::Hidden:
				ShowCursor(FALSE);
				ClipCursor(nullptr);
				break;
			case CursorMode::Locked:
				ShowCursor(FALSE);
				RECT rect;
				GetClientRect(m_Window, &rect);
				MapWindowPoints(m_Window, nullptr, (POINT*)&rect, 2);
				ClipCursor(&rect);
				break;
			}
			m_CursorMode = mode;
		}

		CursorMode GetCursorMode() const
		{
			return m_CursorMode;
		}
	};
}