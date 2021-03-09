#pragma once

#include "MouseCodes.h"
#include "KeyCodes.h"


#include "vhpch.h"

namespace Valhalla
{
	class Input
	{
		static bool IsKeyPressed(KeyCode keycode);

		static bool IsMouseButtonPressed(MouseButton button);
		static float_t GetMouseX();
		static float_t GetMouseY();
		static std::pair<float_t, float_t> GetMousePosition();

		/*static void SetCursorMode(CursorMode mode);
		static CursorMode GetCursorMode();*/
	};
}