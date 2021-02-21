#pragma once
#include <Windows.h>
#include "Types.h"
namespace Valhalla
{
	enum class CursorMode
	{
		Normal = 0,
		Hidden = 1,
		Locked = 2
	};


	typedef enum class MouseButton : uint16_t
	{
		ButtonLeft = VK_LBUTTON,
		ButtonRight = VK_RBUTTON,
		ButtomMiddle = VK_MBUTTON
	};

	inline std::ostream& operator<<(std::ostream& os, MouseButton keycode)
	{
		os << static_cast<int32_t>(keycode);
		return os;
	}
}