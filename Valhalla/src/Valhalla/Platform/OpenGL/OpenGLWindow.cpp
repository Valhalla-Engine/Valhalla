#include "vhpch.h"
#include <Glad/glad.h>
#include "OpenGLWindow.h"

#include "Valhalla/Core/Events/ApplicationEvent.h"
#include "Valhalla/Core/Events/KeyEvent.h"
#include "Valhalla/Core/Events/MouseEvent.h"

namespace Valhalla
{

#ifdef VH_PLATFORM_OTHER
	Window* Window::Create(const WindowProps& props)
	{
		return new Yggdrasil::OpenGLWindow(props);
	}


	namespace Yggdrasil
	{
		static void GLFWErrorCallback(int error, const char* description)
		{
			VH_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
		}

		static bool s_GLFWInitialized = false;

		OpenGLWindow::OpenGLWindow(const WindowProps& props)
		{
			Init(props);
		}

		OpenGLWindow::~OpenGLWindow()
		{
			Shutdown();
		}

		void OpenGLWindow::Init(const WindowProps& props)
		{
			m_Data.Title = props.Title;
			m_Data.Width = props.Width;
			m_Data.Height = props.Height;

			VH_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

			if (!s_GLFWInitialized)
			{
				// TODO: glfwTerminate on system shutdown
				int success = glfwInit();
				VH_CORE_ASSERT(success, "Could not intialize GLFW!");
				glfwSetErrorCallback(GLFWErrorCallback);

				s_GLFWInitialized = true;
			}

			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			glfwMakeContextCurrent(m_Window);
			int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			glfwSetWindowUserPointer(m_Window, &m_Data);

			glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int32_t width, int32_t height)
				{
					auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

					WindowResizeEvent event((uint32_t)width, (uint32_t)height);
					data.EventCallback(event);
					data.Width = width;
					data.Height = height;
			});

			glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) 
			{
					auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

					WindowCloseEvent event;
					data.EventCallback(event);
			});

			glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods) 
			{
					auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

					switch (action)
					{
						case GLFW_PRESS:
						{
							KeyPressedEvent event((KeyCode)key, 0);
							data.EventCallback(event);
							break;
						}
						
						case GLFW_RELEASE:
						{
							KeyReleasedEvent event((KeyCode)key);
							data.EventCallback(event);
							break;
						}

						case GLFW_REPEAT:
						{
							KeyPressedEvent event((KeyCode)key, 1);
							data.EventCallback(event);
							break;
						}
					}
			});

			glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t codepoint) 
			{
					auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

					KeyTypedEvent event((KeyCode)codepoint);
					data.EventCallback(event);
			});

			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int32_t button, int32_t action, int32_t mods) 
			{
					auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

					switch (action)
					{
						case GLFW_PRESS:
						{
							MouseButtonPressedEvent event((MouseCode)button);
							data.EventCallback(event);
							break;

						}
						case GLFW_RELEASE:
						{
							MouseButtonReleasedEvent event((MouseCode)button);
							data.EventCallback(event);
							break;
						}
					}
			});

			glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double_t xOffset, double_t yOffset) 
			{
					auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

					MouseScrolledEvent event((float_t)xOffset, (float_t)yOffset);
					data.EventCallback(event);
			});

			glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double_t x, double_t y) 
			{
					auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

					MouseMovedEvent event((float_t)x, (float_t)y);
					data.EventCallback(event);
			});
		}


		void OpenGLWindow::Shutdown()
		{
			glfwDestroyWindow(m_Window);
		}

		void OpenGLWindow::OnUpdate()
		{
			glfwPollEvents();
			glfwSwapBuffers(m_Window);
		}

		void OpenGLWindow::SetVSync(bool enabled)
		{
			if (enabled)
				glfwSwapInterval(1);
			else
				glfwSwapInterval(0);

			m_Data.VSync = enabled;
		}

		bool OpenGLWindow::IsVSync() const
		{
			return m_Data.VSync;
		}
	}

#endif
}