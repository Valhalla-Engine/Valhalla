#include "vhpch.h"
#ifdef VH_PLATFORM_OTHER
#include "Valhalla/Core/Input.h"
#include "OpenGLWindow.h"

#include "Valhalla/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Valhalla
{
	bool Input::IsKeyPressed(KeyCode keycode)
	{
		auto& window = static_cast<Yggdrasil::OpenGLWindow&>(Application::Get().GetWindow());
		auto state = glfwGetKey(static_cast<GLFWwindow*>(window.GetNativeWindow()), static_cast<int32_t>(keycode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		auto& window = static_cast<Yggdrasil::OpenGLWindow&>(Application::Get().GetWindow());

		auto state = glfwGetMouseButton(static_cast<GLFWwindow*>(window.GetNativeWindow()), button);
		return state == GLFW_PRESS;
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return (float)x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return (float)y;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		auto& window = static_cast<Yggdrasil::OpenGLWindow&>(Application::Get().GetWindow());

		double x, y;
		glfwGetCursorPos(static_cast<GLFWwindow*>(window.GetNativeWindow()), &x, &y);
		return { (float)x, (float)y };

	}
}


#endif // VH_PLATFORM_OTHER
