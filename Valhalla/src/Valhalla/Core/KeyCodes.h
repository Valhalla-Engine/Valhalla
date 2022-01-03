#pragma once
#include <WinUser.h>
namespace Valhalla
{
	typedef enum class KeyCode : uint16_t
	{
#ifdef VH_PLATFORM_WINDOWS
		Space = VK_SPACE,
		//Apostrophe = 39,
		Comma = VK_OEM_COMMA,
		Minus = VK_OEM_MINUS,
		Period = VK_OEM_PERIOD,
		Slash = VK_OEM_2,

		D0 = 48,
		D1 = 49,
		D2 = 50,
		D3 = 51,
		D4 = 52,
		D5 = 53,
		D6 = 54,
		D7 = 55,
		D8 = 56,
		D9 = 57,
		
		SemiColon = VK_OEM_1,
		
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
		
		LeftBracket = VK_OEM_4,
		BackSlash = VK_OEM_5,
		RightBracket = VK_OEM_6,
		GraveAccent = VK_OEM_3,
		
		BackSpace = VK_BACK,
		Enter = VK_RETURN,
		Tab = VK_TAB,
		Pause = VK_PAUSE,
		NumLock = VK_NUMLOCK,
		ScrollLock = VK_SCROLL,
		CapsLock = VK_CAPITAL,
		Escape = VK_ESCAPE,
		PageUp = VK_PRIOR,
		PageDown = VK_NEXT,
		End = VK_END,
		Home = VK_HOME,
		Left = VK_LEFT,
		Up = VK_UP,
		Right = VK_RIGHT,
		Down = VK_DOWN,
		PrintScreen = VK_SNAPSHOT,
		Insert = VK_INSERT,
		Delete = VK_DELETE,
		F1 = VK_F1,
		F2 = VK_F2,
		F3 = VK_F3,
		F4 = VK_F4,
		F5 = VK_F5,
		F6 = VK_F6,
		F7 = VK_F7,
		F8 = VK_F8,
		F9 = VK_F9,
		F10 = VK_F10,
		F11 = VK_F11,
		F12 = VK_F12,
		F13 = VK_F13,
		F14 = VK_F14,
		F15 = VK_F15,
		F16 = VK_F16,
		F17 = VK_F17,
		F18 = VK_F18,
		F19 = VK_F19,
		F20 = VK_F20,
		F21 = VK_F21,
		F22 = VK_F22,
		F23 = VK_F23,
		F24 = VK_F24,
		
		KP0 = VK_NUMPAD0,
		KP1 = VK_NUMPAD1,
		KP2 = VK_NUMPAD2,
		KP3 = VK_NUMPAD3,
		KP4 = VK_NUMPAD4,
		KP5 = VK_NUMPAD5,
		KP6 = VK_NUMPAD6,
		KP7 = VK_NUMPAD7,
		KP8 = VK_NUMPAD8,
		KP9 = VK_NUMPAD9,
		KPMultiply = VK_MULTIPLY,
		KPAdd = VK_ADD,
		KPEqual = VK_SEPARATOR,
		KPSubtract = VK_SUBTRACT,
		KPDecimal = VK_DECIMAL,
		KPDivide = VK_DIVIDE,
		
		LeftShift = VK_LSHIFT,
		RightShift = VK_RSHIFT,
		LeftControl = VK_LCONTROL,
		RightControl = VK_RCONTROL,
		LeftAlt = VK_LMENU,
		RightAlt = VK_RMENU

	#elif defined(VH_PLATFORM_LINUX)
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348

	#endif
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keycode)
	{
		os << static_cast<int32_t>(keycode);
		return os;
	}
}