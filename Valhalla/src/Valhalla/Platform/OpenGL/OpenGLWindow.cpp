#include "vhpch.h"
#include <Glad/glad.h>
#include "OpenGLWindow.h"

#include "Valhalla/Core/Events/ApplicationEvent.h"
#include "Valhalla/Core/Events/KeyEvent.h"
#include "Valhalla/Core/Events/MouseEvent.h"

#include "Valhalla/Renderer/RendererAPI.h"

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

			if (Heimdal::RendererAPI::Current() == Heimdal::RendererAPIType::None)
				glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

			m_Window = glfwCreateWindow((int32_t)props.Width, (int32_t)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			
			m_RendererContext = Heimdal::RendererContext::Create(m_Window);
			m_RendererContext->Create();

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

		inline std::pair<float_t, float_t> OpenGLWindow::GetWindowPos() const
		{
			int32_t x, y;
			glfwGetWindowPos(m_Window, &x, &y);
			return { x, y };
		}

		void OpenGLWindow::ProcessEvents()
		{
			glfwPollEvents();
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		void OpenGLWindow::SwapBuffers()
		{
			m_RendererContext->SwapBuffers();
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

		void OpenGLWindow::SetTitle(const std::string& title)
		{
			m_Data.Title = title;
			glfwSetWindowTitle(m_Window, m_Data.Title.c_str());
		}
	}

#endif
}