#pragma once

#include "Valhalla/Core/Window.h"
#include "Valhalla/Renderer/RendererContext.h"

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

			virtual void ProcessEvents() override;
			virtual void SwapBuffers() override;

			inline unsigned int GetWidth() const override { return m_Data.Width; }
			inline unsigned int GetHeight() const override { return m_Data.Height; }

			virtual std::pair<uint32_t, uint32_t> GetSize() const override { return { m_Data.Width, m_Data.Height }; }
			virtual std::pair<float_t, float_t> GetWindowPos() const override;

			// Window attributes
			virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
			virtual void SetVSync(bool enabled) override;
			virtual bool IsVSync() const override;

			virtual const std::string& GetTitle() const override { return m_Data.Title; }
			virtual void SetTitle(const std::string& title) override;

			inline void* GetNativeWindow() const { return m_Window; }

			virtual Ref<Heimdal::RendererContext> GetRenderContext() override { return m_RendererContext; }


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

			Ref<Heimdal::RendererContext> m_RendererContext;
		};

#endif
	}
}