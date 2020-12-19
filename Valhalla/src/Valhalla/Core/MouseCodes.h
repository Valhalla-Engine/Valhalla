#pragma once

#include <GLFW/glfw3.h>

#ifdef VH_PLATFORM_WINDOWS

#include <Windows.h>
namespace Valhalla
{
	typedef enum class VALHALLA_API MouseCode : uint8_t
	{
		LeftMouseButton = VK_LBUTTON,
		RightMouseButton = VK_RBUTTON,
		MiddleMouseButton = VK_MBUTTON,
		ThumbMouseButton = VK_XBUTTON1,
		ThumbMouseButton2 = VK_XBUTTON2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}
#else
namespace Valhalla
{
	typedef enum class VALHALLA_API MouseCode : uint8_t
	{
		LeftMouseButton = 1,
		RightMouseButton = 2,
		MiddleMouseButton = 3,
		ThumbMouseButton = 5,
		ThumbMouseButton2 = 6
	}Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}
#endif