#pragma once
#include "vhpch.h"
#include "KeyCodes.h"
#include "MouseCodes.h"
#include <map>

namespace Valhalla
{
	enum class CursorMode
	{
		Normal = 0,
		Hidden = 1,
		Locked = 2
	};

	struct Controller
	{
		int32_t ID;
		std::string Name;
		std::map<int32_t, bool> ButtonStates;
		std::map<int32_t, float_t> AxisStatees;
		std::map<int32_t, uint8_t> HatStates;
	};

	class Input
	{
	public:
		static void Update();

		static bool IsKeyPressed(KeyCode keycode);

		static bool IsMouseButtonPressed(MouseButton button);
		static float_t GetMouseX();
		static float_t GetMouseY();
		static std::pair<float_t, float_t> GetMousePosition();

		static void SetCursorMode(CursorMode mode);
		static CursorMode GetCursormMode();


		static bool IsControllerPresent(int32_t id);
		static std::vector<int32_t> GetConnectedControllerIDs();
		static const Controller* GetController(int32_t id);
		static std::string_view GetControllerName(int32_t id);
		static bool IsControllerButtonPressed(int32_t controllerID, int32_t button);
		static float_t GetControllerAxis(int32_t controllerID, int32_t axis);
		static uint8_t GetControllerHat(int32_t controllerID, int32_t hat);

		static const std::map<int32_t, Controller>& GetControllers() { return s_Controllers; }
	private:
		inline static std::map<int32_t, Controller> s_Controllers;
	};
}