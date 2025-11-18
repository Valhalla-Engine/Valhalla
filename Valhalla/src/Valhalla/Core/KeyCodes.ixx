#include <Windows.h>

export module Valhalla.Core.KeyCodes;

import Valhalla.Core.Types;
import <ostream>;

export namespace Valhalla
{
	typedef enum class KeyCode : uint16_t
	{
		Space = VK_SPACE,
		Comma = VK_OEM_COMMA,
		Minus = VK_OEM_MINUS,
		Period = VK_OEM_PERIOD,
		Slash = VK_OEM_2,

		D0 = 0x30,
		D1 = 0x31,
		D2 = 0x32,
		D3 = 0x33,
		D4 = 0x34,
		D5 = 0x35,
		D6 = 0x36,
		D7 = 0x37,
		D8 = 0x38,
		D9 = 0x39,

		SemiColon = VK_OEM_1,

		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,

		LeftBracket = VK_OEM_4,
		BackSlash = VK_OEM_5,
		RightBracket = VK_OEM_6,
		GraveAccent = VK_OEM_3,

		/* Function keys */
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

		/* Keypad */
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
	}Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keycode)
	{
		os << static_cast<int32_t>(keycode);
		return os;
	}
}
