#pragma once

#include <functional>

#include "Base.h"
#include "Events/Event.h"

#include "Valhalla/Renderer/RendererContext.h"
#include "Valhalla/Renderer/SwapChain.h"

namespace Valhalla
{
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Valhalla Engine",
						uint32_t width = 1280,
						uint32_t height = 720)
			: Title(title), Width(width), Height(height) {}
	};

	// interface for desktop window
	class Window : public RefCounted
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void ProcessEvents() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual std::pair<uint32_t, uint32_t> GetSize() const = 0;
		virtual std::pair<float_t, float_t> GetWindowPos() const = 0;

		virtual void Maximize() = 0;

		// Window Attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool_t enabled) = 0;
		virtual bool_t IsVSync() const = 0;

		virtual const std::string& GetTitle() const = 0;
		virtual void SetTitle(const std::string& title) = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual void SwapBuffers() = 0;

		virtual void Init() = 0;

		virtual Ref<Heimdall::RendererContext> GetRendererContext() = 0;
		virtual Ref<Heimdall::SwapChain> GetSwapChain() = 0;
		
		static Window* Create(const WindowProps& props = WindowProps());
	};
}