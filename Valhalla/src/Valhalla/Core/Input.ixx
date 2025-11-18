export module Valhalla.Core.Input;

import Valhalla.Core.KeyCodes;
import Valhalla.Core.MouseCodes;

import <utility>;
import <memory>;


namespace Valhalla::Input
{
	export struct IInputBackend
	{
		virtual ~IInputBackend() = default;

		virtual bool IsKeyPressed(KeyCode keycode) = 0;
		
		virtual bool IsMouseButtonPressed(MouseButton button) = 0;
		virtual float_t GetMouseX() = 0;
		virtual float_t GetMouseY() = 0;
		virtual std::pair<float_t, float_t> GetMousePosition() = 0;

		virtual void SetCursorMode(CursorMode mode) = 0;
		virtual CursorMode GetCursorMode() = 0;
	};

	export void Init(std::unique_ptr<IInputBackend> backends);
	export void Shutdown();
	
	export bool IsKeyPressed(KeyCode keycode);
	export bool IsMouseButtonPressed(MouseButton button);
	export float_t GetMouseX();
	export float_t GetMouseY();
	export std::pair<float_t, float_t> GetMousePosition();
	
	export void SetCursorMode(CursorMode mode);
	export CursorMode GetCursorMode();
}