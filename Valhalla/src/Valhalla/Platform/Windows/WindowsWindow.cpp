module;
#include "ImGui/imgui.h"

#include <Windows.h>
#include <windowsx.h>

#include <locale>
#include <codecvt>


module Valhalla.Platform.Windows.Window;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

import Valhalla.Core.Log;
import Valhalla.Core.Assert;
import Valhalla.Core.Input;
import Valhalla.Core.Application;

import Valhalla.Core.Events.ApplicationEvent;
import Valhalla.Core.Events.MouseEvents;
import Valhalla.Core.Events.KeyEvent;

import Valhalla.Core.MouseCodes;
import Valhalla.Core.KeyCodes;

namespace Valhalla
{
	static int WindowCreatingBlocking = 0;


	static void Win32ErrorCallback(int32_t error, const int8_t* description)
	{
		Log::CoreError("Win32 Error ({0}): {1}", error, description);
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
		Log::CoreInfo("Creating Window {0} {1}x{2}", m_Data.Title, m_Data.Width, m_Data.Height);

		m_hInstance = GetModuleHandle(nullptr);

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
			Log::CoreError("Could not initialize the window");
		}

		int32_t posX = (GetSystemMetrics(SM_CXSCREEN) - m_Data.Width) / 2;
		int32_t posY = (GetSystemMetrics(SM_CYSCREEN) - m_Data.Height) / 2;

		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::wstring wideName = converter.from_bytes(m_Data.Title);
		m_Window = CreateWindow(wc.lpszClassName,wideName.c_str(), WS_OVERLAPPEDWINDOW, posX, posY, m_Data.Width, m_Data.Height, NULL, NULL, wc.hInstance, NULL);

		if (!m_Window)
		{
			Assert(m_Window, "Could not initialize Win32");
		}

		m_RendererContext = Heimdall::RendererContext::Create();
		m_SwapChain = Heimdall::SwapChain::Create();
		m_RendererContext->Init();

		m_SwapChain->Init(m_RendererContext);
		m_SwapChain->InitSurface();
		m_SwapChain->CreateSwapchain(&m_Data.Width, &m_Data.Height, m_Data.VSync);

		SetWindowLongPtr(m_Window, GWLP_USERDATA, (LONG_PTR)&m_Data);
		ShowWindow(m_Window, SW_SHOW);
		UpdateWindow(m_Window);
		SetFocus(m_Window);
	}

	void WindowsWindow::Shutdown()
	{
		if (m_SwapChain)
		{
			//m_SwapChain->Shutdown();
			m_SwapChain = nullptr;
		}

		if (m_RendererContext)
		{
			//m_RendererContext->Shutdown();
			m_RendererContext = nullptr;
		}

		if (m_Window)
		{
			DestroyWindow(m_Window);
			m_Window = nullptr;
		}

		if (m_hInstance)
		{
			UnregisterClass(L"Win32 Window", m_hInstance);
			m_hInstance = nullptr;
		}
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
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
				data->Width = (UINT)LOWORD(lparam);
				data->Height = (UINT)HIWORD(lparam);

				
				//Application::Get().GetWindow().GetSwapChain()->OnResize(data->Width, data->Height);

				WindowResizeEvent event((UINT)LOWORD(lparam), (UINT)HIWORD(lparam));
				data->EventCallback(event);
				return true;
			}

			case WM_CLOSE:
			case WM_DESTROY:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

				WindowCloseEvent event;
				data->EventCallback(event);
				return true;
			}

			case WM_KEYUP:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

				KeyReleasedEvent event(static_cast<KeyCode>(wparam));
				data->EventCallback(event);
				return true;
			}

			case WM_CHAR:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

				KeyTypedEvent event(static_cast<KeyCode>(wparam));
				data->EventCallback(event);
				return true;
			}

			case WM_KEYDOWN:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
				int32_t repeatCount = (lparam & 0xffff);

				KeyPressedEvent event(static_cast<KeyCode>(wparam), repeatCount);
				data->EventCallback(event);
				return true;
			}

			case WM_MOUSEMOVE:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
				
				MouseMovedEvent event((float_t)GET_X_LPARAM(lparam), (float_t)GET_Y_LPARAM(lparam));
				data->EventCallback(event);
				return true;
			}

			case WM_MOUSEWHEEL:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

				MouseScrolledEvent event((float_t)GET_WHEEL_DELTA_WPARAM(wparam) / (float_t)WHEEL_DELTA, 0.0f);
				data->EventCallback(event);
				return true;
			}

			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			{
				WindowData* data = (WindowData*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

				Valhalla::MouseButton button;
				switch (msg)
				{
				case WM_LBUTTONDOWN: case WM_LBUTTONUP: button = MouseButton::ButtonLeft; break;
				case WM_MBUTTONDOWN: case WM_MBUTTONUP: button = MouseButton::ButtonMiddle; break;
				case WM_RBUTTONDOWN: case WM_RBUTTONUP: button = MouseButton::ButtonRight; break;
				}

				if (msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN)
				{
					MouseButtonPressedEvent event(button);
					data->EventCallback(event);
				}
				else
				{
					MouseButtonReleasedEvent event(button);
					data->EventCallback(event);
				}
				return true;
			}

			default:
				return DefWindowProc(hwnd, msg, wparam, lparam);
		}
	}
}