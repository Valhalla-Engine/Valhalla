#include "vhpch.h"
#include "Valhalla/Core/Input.h"
#include "WindowsWindow.h"

#include "Valhalla/Core/Application.h"

namespace Valhalla
{
	bool_t Input::IsKeyPressed(KeyCode keycode)
	{
		auto state = GetAsyncKeyState(static_cast<int32_t>(keycode));

		return (state & 0x8000);
	}

	bool_t Input::IsMouseButtonPressed(MouseButton button)
	{
		auto state = GetAsyncKeyState(static_cast<int32_t>(button));

		return (state & 0x8000);
	}

	std::pair<float_t, float_t> Input::GetMousePosition()
	{
		POINT p;

		GetCursorPos(&p);

		return { (float_t)p.x, (float_t)p.y };
	}

	float_t Input::GetMouseX()
	{
		auto [x, y] = Input::GetMousePosition();

		return x;
	}

	float_t Input::GetMouseY()
	{
		auto [x, y] = Input::GetMousePosition();

		return y;
	}
}