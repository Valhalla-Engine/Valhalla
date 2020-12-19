#pragma once

#include "Valhalla/Core/Window.h"

#include <GLFW/glfw3.h>

namespace Valhalla
{
	namespace Yggdrasil
	{

#ifdef VH_PLATFORM_OTHER
		class VALHALLA_API OpenGLWindow : public Window
		{
		public:
			OpenGLWindow(const WindowProps& props);
			virtual ~OpenGLWindow();

			void OnUpdate() override;

			inline uint32_t GetWidth() const override { return m_Data.Width; }
			inline uint32_t GetHeight() const override { return m_Data.Height; }

			// Window attributes
			virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
			virtual void SetVSync(bool enabled) override;
			virtual bool IsVSync() const override;

			inline void* GetNativeWindow() const { return m_Window; }

		private:
			virtual void Init(const WindowProps& props);
			virtual void Shutdown();

		private:
			GLFWwindow* m_Window;
			
			struct WindowData
			{
				std::string Title;
				uint32_t Width, Height;
				bool VSync;

				EventCallbackFn EventCallback;
			};

			WindowData m_Data;
			float_t m_LastFrameTime = 0.0f;
		};

#endif
	}
}