#pragma once

#include "Valhalla/Core/Window.h"

namespace Valhalla
{
#ifdef VH_PLATFORM_WINDOWS

#include <Windows.h>

	namespace Yggdrasil
	{
		class VALHALLA_API WindowsWindow : public Window
		{
		public:
			WindowsWindow(const WindowProps& props);
			virtual ~WindowsWindow();

			void OnUpdate() override;

			inline uint32_t GetWidth() const override { return m_Data.Width; }
			inline uint32_t GetHeight() const override { return m_Data.Height; }

			void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
			void SetVSync(bool enabled) override;
			bool IsVSync() const override;

			inline virtual void* GetNativeWindow() const override { return m_Window; }

			static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		private:
			virtual void Init(const WindowProps& props);
			virtual void Shutdown();
		private:
			HWND m_Window;
			HINSTANCE m_hInstance;

			struct WindowData
			{
				std::string Title = "";
				uint32_t Width = 0, Height = 0;
				bool VSync = false;

				EventCallbackFn EventCallback;
			};

			WindowData m_Data;
		};
	}
#endif
}