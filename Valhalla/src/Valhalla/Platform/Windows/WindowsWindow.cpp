#include "vhpch.h"
#include "WindowsWindow.h"

#include "Valhalla/Core/Base.h"
#include "Valhalla/Core/Assert.h"

#include "Valhalla/Core/Input.h"

#include "Valhalla/Core/Application.h"

#include "Valhalla/Core/Events/ApplicationEvent.h"
#include "Valhalla/Core/Events/KeyEvent.h"
#include "Valhalla/Core/Events/MouseEvent.h"

#include "ImGui/imgui.h"

#include <Windowsx.h>

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Valhalla
{
	static int WindowCreatingBlocking = 0;

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	static void Win32ErrorCallback(int32_t error, const int8_t* description)
	{
		VH_CORE_ERROR("Win32 Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init()
	{
		VH_CORE_INFO("Creating Window {0} {1}x{2}", m_Data.Title, m_Data.Width, m_Data.Height);

		m_hInstance = GetModuleHandle(0);

		WNDCLASSEX wc = {};
		wc.lpfnWndProc = WindowProc;
		wc.style = CS_CLASSDC;
		wc.hInstance = m_hInstance;
		wc.lpszClassName = "Win32 Window";
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
			VH_CORE_ERROR("Could not initialize the window");
		}

		m_Window = CreateWindow(wc.lpszClassName, m_Data.Title.c_str(), WS_OVERLAPPEDWINDOW, 0, 0, m_Data.Width, m_Data.Height, NULL, NULL, wc.hInstance, NULL);

		if (!m_Window)
		{
			VH_CORE_ASSERT(m_Window, "Could not initialize Win32");
		}

		SetWindowLongPtr(m_Window, 0, (LONG_PTR)&m_Data);
		ShowWindow(m_Window, SW_SHOW);
		UpdateWindow(m_Window);
		SetFocus(m_Window);

		m_RendererContext = Heimdall::RendererContext::Create();
		m_SwapChain = Heimdall::SwapChain::Create();
		m_RendererContext->Init();
	}

	void WindowsWindow::Shutdown()
	{
		DestroyWindow(m_Window);
		m_SwapChain->~SwapChain();
		m_RendererContext->~RendererContext();
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

	void WindowsWindow::SetVSync(bool_t enabled)
	{
		m_Data.VSync = enabled;
	}

	bool_t WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::SetTitle(const std::string& title)
	{
		m_Data.Title = title;
		SetWindowText(m_Window, m_Data.Title.c_str());
	}

	std::pair<float_t, float_t> WindowsWindow::GetWindowPos() const
	{
		RECT t;
		GetWindowRect(m_Window, &t);
		return {t.left, t.top};
	}

	void WindowsWindow::Maximize()
	{
		SetWindowPos(m_Window, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
	}

	LRESULT CALLBACK WindowsWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		LRESULT result = NULL;

		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
			return true;

		switch (msg)
		{
			case WM_SIZE:
			{
				if (WindowCreatingBlocking > 0)
				{
					WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);
					data->Width = (UINT)LOWORD(lparam);
					data->Height = (UINT)HIWORD(lparam);

					WindowResizeEvent event((UINT)LOWORD(lparam), (UINT)HIWORD(lparam));
					data->EventCallback(event);
				}

				WindowCreatingBlocking++;

				break;
			}

			case WM_CLOSE:
			case WM_DESTROY:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);

				WindowCloseEvent event;
				data->EventCallback(event);
				break;
			}

			case WM_KEYUP:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);

				KeyReleasedEvent event(static_cast<KeyCode>(wparam));
				data->EventCallback(event);
				break;
			}

			case WM_CHAR:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);

				KeyTypedEvent event(static_cast<KeyCode>(wparam));
				data->EventCallback(event);
				break;
			}

			case WM_KEYDOWN:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);
				int32_t repeatCount = (lparam & 0xffff);

				KeyPressedEvent event(static_cast<KeyCode>(lparam), repeatCount);
				data->EventCallback(event);
				break;
			}

			case WM_MOUSEMOVE:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);
				
				MouseMovedEvent event((float_t)GET_X_LPARAM(lparam), (float_t)GET_Y_LPARAM(lparam));
				data->EventCallback(event);
				break;
			}

			case WM_MOUSEWHEEL:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);

				MouseScrolledEvent event((float_t)GET_WHEEL_DELTA_WPARAM(wparam) / (float_t)WHEEL_DELTA, 0.0f);
				data->EventCallback(event);
				break;
			}

			case WM_LBUTTONDOWN:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);

				MouseButtonPressedEvent event(MouseButton::ButtonLeft);
				data->EventCallback(event);
				break;
			}

			case WM_LBUTTONUP:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);

				MouseButtonReleasedEvent event(MouseButton::ButtonLeft);
				data->EventCallback(event);
				break;
			}

			case WM_MBUTTONDOWN:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);

				MouseButtonPressedEvent event(MouseButton::ButtonMiddle);
				data->EventCallback(event);
				break;
			}

			case WM_MBUTTONUP:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);

				MouseButtonReleasedEvent event(MouseButton::ButtonMiddle);
				data->EventCallback(event);
				break;
			}

			case WM_RBUTTONDOWN:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);

				MouseButtonPressedEvent event(MouseButton::ButtonRight);
				data->EventCallback(event);
				break;
			}

			case WM_RBUTTONUP:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);

				MouseButtonReleasedEvent event(MouseButton::ButtonRight);
				data->EventCallback(event);
				break;
			}

			default:
				result = DefWindowProc(hwnd, msg, wparam, lparam);
		}

		return result;
	}
}