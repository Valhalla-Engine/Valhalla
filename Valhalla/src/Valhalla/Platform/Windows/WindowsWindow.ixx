module;
#include <Windows.h>

export module Valhalla.Platform.Windows.Window;

import Valhalla.Core.Window;


export namespace Valhalla
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void ProcessEvents() override;

		inline uint32_t GetWidth() const override { return m_Data.Width; }
		inline uint32_t GetHeight() const override { return m_Data.Height; }

		virtual std::pair<uint32_t, uint32_t> GetSize() const override { return { m_Data.Width, m_Data.Height }; }
		virtual std::pair<float_t, float_t> GetWindowPos() const override;

		// Window Attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool_t enabled) override;
		virtual bool_t IsVSync() const override;

		virtual void Maximize() override;

		virtual const std::string& GetTitle() const override { return m_Data.Title; }
		virtual void SetTitle(const std::string& title) override;

		inline void* GetNativeWindow() const { return m_Window; }

		virtual Ref<Heimdall::RendererContext> GetRendererContext() override { return m_RendererContext; }
		virtual Ref<Heimdall::SwapChain> GetSwapChain() override { return m_SwapChain; }

		virtual void SwapBuffers() override;

		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
		virtual void Init() override;
	private:
		virtual void Shutdown();
	private:
		HWND m_Window;

		HINSTANCE m_hInstance;

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
		float_t m_LastFrameTime = 0.0f;
		Ref<Heimdall::RendererContext> m_RendererContext;
		Ref<Heimdall::SwapChain> m_SwapChain;
	};
}