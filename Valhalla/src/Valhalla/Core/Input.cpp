module Valhalla.Core.Input;

import <memory>;

namespace
{
	std::unique_ptr<Valhalla::Input::IInputBackend> g_backend;
}

namespace Valhalla::Input
{
	void Init(std::unique_ptr<IInputBackend> backend)
	{
		g_backend = std::move(backend);
	}

	void Shutdown()
	{
		g_backend.reset();	
	}

	bool IsKeyPressed(KeyCode keycode)
	{
		return g_backend->IsKeyPressed(keycode);
	}

	bool IsMouseButtonPressed(MouseButton button)
	{
		return g_backend->IsMouseButtonPressed(button);
	}

	float_t GetMouseX()
	{
		return g_backend->GetMouseX();
	}

	float_t GetMouseY()
	{
		return g_backend->GetMouseY();
	}

	std::pair<float_t, float_t> GetMousePosition()
	{
		return g_backend->GetMousePosition();
	}

	void SetCursorMode(CursorMode mode)
	{
		g_backend->SetCursorMode(mode);
	}

	CursorMode GetCursorMode()
	{
		return g_backend->GetCursorMode();
	}
}