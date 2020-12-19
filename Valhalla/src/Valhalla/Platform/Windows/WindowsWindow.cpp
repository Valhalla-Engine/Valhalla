#include "vhpch.h"
#include "WindowsWindow.h"

#include "Valhalla/Core/Events/ApplicationEvent.h"
#include "Valhalla/Core/Events/KeyEvent.h"
#include "Valhalla/Core/Events/MouseEvent.h"

//#include "imgui.h"

#include <windowsx.h>

//extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

namespace Valhalla
{
#ifdef VH_PLATFORM_WINDOWS

	Window* Window::Create(const WindowProps& props)
	{
		return new Yggdrasil::WindowsWindow(props);
	}



	namespace Yggdrasil
	{
		static bool s_Win32Initialized = false;

		static int windowCreationBlocking = 0;

		WindowsWindow::WindowsWindow(const WindowProps& props)
		{
			Init(props);
		}

		WindowsWindow::~WindowsWindow()
		{
			Shutdown();
		}

		void WindowsWindow::Init(const WindowProps& props)
		{


			m_Data.Title = props.Title;
			m_Data.Width = props.Width;
			m_Data.Height = props.Height;

			m_hInstance = GetModuleHandle(0);

			WNDCLASSEX wc = {};
			wc.lpfnWndProc = WindowProc;
			wc.style = CS_CLASSDC;
			wc.hInstance = m_hInstance;
			wc.lpszClassName = "Valhalla Win32 Window";
			wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
			wc.hIconSm = wc.hIcon;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = sizeof(WindowData*);
			wc.lpszMenuName = NULL;
			wc.cbSize = sizeof(WNDCLASSEX);

			if (!RegisterClassEx(&wc))
			{
				VH_CORE_ERROR("Could not initialize the window class!");
			}

			//m_Window = CreateWindow(wc.lpszClassName, "Valhalla Engine", WS_OVERLAPPEDWINDOW, 0, 0, m_Data.Width, m_Data.Height, NULL, NULL, wc.hInstance, NULL);

			m_Window = CreateWindowEx(WS_EX_APPWINDOW, wc.lpszClassName, "Valhalla Engine",
									WS_OVERLAPPEDWINDOW,
									0, 0, m_Data.Width, m_Data.Height, NULL, NULL, m_hInstance, NULL);

			if (!s_Win32Initialized)
			{
				VH_CORE_ASSERT(m_Window, "Could not initialize Win32!");

				s_Win32Initialized = true;
			}

			SetWindowLongPtr(m_Window, 0, (LONG_PTR)&m_Data);
			ShowWindow(m_Window, SW_SHOWDEFAULT);
			UpdateWindow(m_Window);
			SetFocus(m_Window);
		}

		void WindowsWindow::Shutdown()
		{
			DestroyWindow(m_Window);
		}

		void WindowsWindow::OnUpdate()
		{
			MSG message;

			while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE) > 0)
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}

		void WindowsWindow::SetVSync(bool enabled)
		{
			m_Data.VSync = enabled;
		}

		bool WindowsWindow::IsVSync() const
		{
			return m_Data.VSync;
		}

		LRESULT CALLBACK WindowsWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
		{
			LRESULT result = NULL;

			/*if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
				return true; */

			switch (msg)
			{
				case WM_SIZE:
				{
					if (windowCreationBlocking > 0)
					{
						auto& data = *((WindowData*)GetWindowLongPtr(hwnd, 0));
						data.Width = (UINT)LOWORD(lparam);
						data.Height = (UINT)HIWORD(lparam);

						WindowResizeEvent event((UINT)LOWORD(lparam), (UINT)HIWORD(lparam));
						data.EventCallback(event);
					}

					windowCreationBlocking++;
					break;
				}

				case WM_CLOSE:
				case WM_DESTROY:
				{
					auto& data = *((WindowData*)GetWindowLongPtr(hwnd, 0));

					WindowCloseEvent event;
					data.EventCallback(event);
					break;
				}

				case WM_KEYUP:
				{
					auto& data = *((WindowData*)GetWindowLongPtr(hwnd, 0));

					KeyReleasedEvent event(static_cast<KeyCode>(wparam));
					data.EventCallback(event);
					break;
				}

				case WM_CHAR:
				{
					auto& data = *((WindowData*)GetWindowLongPtr(hwnd, 0));

					KeyTypedEvent event(static_cast<KeyCode>(wparam));
					data.EventCallback(event);
					break;
				}

				case WM_KEYDOWN:
				{
					auto& data = *((WindowData*)GetWindowLongPtr(hwnd, 0));
					int repeatCount = (lparam & 0xffff);

					KeyPressedEvent event(static_cast<KeyCode>(wparam), repeatCount);
					data.EventCallback(event);
					break;
				}
				case WM_MOUSEMOVE:
				{
					auto& data = *((WindowData*)GetWindowLongPtr(hwnd, 0));

					MouseMovedEvent event((float_t)GET_X_LPARAM(lparam), (float_t)GET_Y_LPARAM(lparam));
					data.EventCallback(event);
					break;
				}
				case WM_MOUSEWHEEL:
				{
					auto& data = *((WindowData*)GetWindowLongPtr(hwnd, 0));

					break;

				}
				case WM_LBUTTONDOWN:
				{
					auto& data = *((WindowData*)GetWindowLongPtr(hwnd, 0));

					MouseButtonPressedEvent event(static_cast<MouseCode>(VK_LBUTTON));
					data.EventCallback(event);
					break;
				}
				case WM_LBUTTONUP:
				{
					auto& data = *((WindowData*)GetWindowLongPtr(hwnd, 0));

					MouseButtonReleasedEvent event(static_cast<MouseCode>(VK_LBUTTON));
					data.EventCallback(event);
					break;
				}
				case WM_MBUTTONDOWN:
				{
					auto& data = *((WindowData*)GetWindowLongPtr(hwnd, 0));

					MouseButtonPressedEvent event(static_cast<MouseCode>(VK_MBUTTON));
					data.EventCallback(event);
					break;
				}
				case WM_MBUTTONUP:
				{
					auto& data = *((WindowData*)GetWindowLongPtr(hwnd, 0));

					MouseButtonReleasedEvent event(static_cast<MouseCode>(VK_MBUTTON));
					data.EventCallback(event);
					break;
				}
				case WM_RBUTTONDOWN:
				{
					auto& data = *((WindowData*)GetWindowLongPtr(hwnd, 0));

					MouseButtonPressedEvent event(static_cast<MouseCode>(VK_RBUTTON));
					data.EventCallback(event);
					break;
				}
				case WM_RBUTTONUP:
				{
					auto& data = *((WindowData*)GetWindowLongPtr(hwnd, 0));

					MouseButtonReleasedEvent event(static_cast<MouseCode>(VK_RBUTTON));
					data.EventCallback(event);
					break;
				}

				case WM_XBUTTONDOWN:
				{
					auto& data = *((WindowData*)GetWindowLongPtr(hwnd, 0));

					MouseButtonPressedEvent event(static_cast<MouseCode>(VK_XBUTTON1 | VK_XBUTTON2));
					data.EventCallback(event);
					break;
				}
				case WM_XBUTTONUP:
				{
					auto& data = *((WindowData*)GetWindowLongPtr(hwnd, 0));

					MouseButtonReleasedEvent event(static_cast<MouseCode>(VK_XBUTTON1 | VK_XBUTTON2));
					data.EventCallback(event);
					break;
				}
				default:
					result = DefWindowProc(hwnd, msg, wparam, lparam);
			}

			return result;
		}
	}

#endif
}