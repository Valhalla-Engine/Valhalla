#include <Windows.h>

export module Valhalla.Core.MouseCodes;


import Valhalla.Core.Types;
import <ostream>;

export namespace Valhalla
{
	enum class CursorMode
	{
		Normal = 0,
		Hidden = 1,
		Locked = 2
	};


	enum class MouseButton : uint16_t
	{
		ButtonLeft = VK_LBUTTON,
		ButtonRight = VK_RBUTTON,
		ButtonMiddle = VK_MBUTTON
	};

	inline std::ostream& operator<<(std::ostream& os, MouseButton keycode)
	{
		os << static_cast<int32_t>(keycode);
		return os;
	}
}