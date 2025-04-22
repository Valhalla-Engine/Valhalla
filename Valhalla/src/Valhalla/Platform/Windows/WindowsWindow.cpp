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

#include <locale>
#include <codecvt>

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
		wc.lpszClassName = L"Win32 Window";
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

		int32_t posX = (GetSystemMetrics(SM_CXSCREEN) - m_Data.Width) / 2;
		int32_t posY = (GetSystemMetrics(SM_CYSCREEN) - m_Data.Height) / 2;

		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::wstring wideName = converter.from_bytes(m_Data.Title);
		m_Window = CreateWindow(wc.lpszClassName,wideName.c_str(), WS_OVERLAPPEDWINDOW, posX, posY, m_Data.Width, m_Data.Height, NULL, NULL, wc.hInstance, NULL);

		if (!m_Window)
		{
			VH_CORE_ASSERT(m_Window, "Could not initialize Win32");
		}

		m_RendererContext = Heimdall::RendererContext::Create();
		m_SwapChain = Heimdall::SwapChain::Create();
		m_RendererContext->Init();

		m_SwapChain->Init(m_RendererContext);
		m_SwapChain->InitSurface();
		m_SwapChain->CreateSwapchain(&m_Data.Width, &m_Data.Height, m_Data.VSync);

		SetWindowLongPtr(m_Window, 0, (LONG_PTR)&m_Data);
		ShowWindow(m_Window, SW_SHOW);
		UpdateWindow(m_Window);
		SetFocus(m_Window);
	}

	void WindowsWindow::Shutdown()
	{
		m_SwapChain->Destroy();
		m_RendererContext->~RendererContext();
		DestroyWindow(m_Window);
	}

	void WindowsWindow::SwapBuffers()
	{
		m_SwapChain->Present();
	}

	void WindowsWindow::ProcessEvents()
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
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::wstring wideName = converter.from_bytes(m_Data.Title);
		SetWindowText(m_Window, wideName.c_str());
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
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
			return true;

		switch (msg)
		{
			case WM_SIZE:
			{
				WindowData* data = reinterpret_cast<WindowData*>(GetWindowLongPtr(hwnd, 0));
				data->Width = (UINT)LOWORD(lparam);
				data->Height = (UINT)HIWORD(lparam);

				
				Application::Get().GetWindow().GetSwapChain()->OnResize(data->Width, data->Height);

				WindowResizeEvent event((UINT)LOWORD(lparam), (UINT)HIWORD(lparam));
				data->EventCallback(event);
				return true;
			}

			case WM_CLOSE:
			case WM_DESTROY:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);

				WindowCloseEvent event;
				data->EventCallback(event);
				return true;
			}

			case WM_KEYUP:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);

				KeyReleasedEvent event(static_cast<KeyCode>(wparam));
				data->EventCallback(event);
				return true;
			}

			case WM_CHAR:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);

				KeyTypedEvent event(static_cast<KeyCode>(wparam));
				data->EventCallback(event);
				return true;
			}

			case WM_KEYDOWN:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);
				int32_t repeatCount = (lparam & 0xffff);

				KeyPressedEvent event(static_cast<KeyCode>(lparam), repeatCount);
				data->EventCallback(event);
				return true;
			}

			case WM_MOUSEMOVE:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);
				
				MouseMovedEvent event((float_t)GET_X_LPARAM(lparam), (float_t)GET_Y_LPARAM(lparam));
				data->EventCallback(event);
				return true;
			}

			case WM_MOUSEWHEEL:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);

				MouseScrolledEvent event((float_t)GET_WHEEL_DELTA_WPARAM(wparam) / (float_t)WHEEL_DELTA, 0.0f);
				data->EventCallback(event);
				return true;
			}

			case WM_LBUTTONDOWN:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);

				MouseButtonPressedEvent event(MouseButton::ButtonLeft);
				data->EventCallback(event);
				return true;
			}

			case WM_LBUTTONUP:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);

				MouseButtonReleasedEvent event(MouseButton::ButtonLeft);
				data->EventCallback(event);
				return true;
			}

			case WM_MBUTTONDOWN:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);

				MouseButtonPressedEvent event(MouseButton::ButtonMiddle);
				data->EventCallback(event);
				return true;
			}

			case WM_MBUTTONUP:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);

				MouseButtonReleasedEvent event(MouseButton::ButtonMiddle);
				data->EventCallback(event);
				return true;
			}

			case WM_RBUTTONDOWN:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);

				MouseButtonPressedEvent event(MouseButton::ButtonRight);
				data->EventCallback(event);
				return true;
			}

			case WM_RBUTTONUP:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, 0);

				MouseButtonReleasedEvent event(MouseButton::ButtonRight);
				data->EventCallback(event);
				return true;
			}

			default:
				return DefWindowProc(hwnd, msg, wparam, lparam);
		}
	}
}