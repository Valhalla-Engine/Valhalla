#pragma once

#include <functional>

#include "Valhalla/Core/Base.h"
#include "Valhalla/Core/Events/Event.h"

namespace Valhalla
{
	struct WindowSpecification
	{
		std::string Title = "Valhalla";
		uint32_t Width = 1600;
		uint32_t Height = 900;
		bool Decorated = true;
		bool Fullscreen = false;
		bool VSync = true;
	};

	
	class Window : public RefCounted
	{
	public:

		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void Init() = 0;
		virtual void ProcessEvents() = 0;
		virtual void SwapBuffers() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual std::pair<uint32_t, uint32_t> GetSize() const = 0;
		virtual std::pair<float_t, float_t> GetWindowPos() const = 0;

		virtual void Maximize() = 0;
		virtual void CenterWindow() = 0;

		// Window Attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual void IsVSync() const = 0;
		virtual void SetResizeable(bool resizeable) const = 0;

		virtual const std::string& GetTitle() const = 0;
		virtual void SetTitle(const std::string& title) = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowSpecification& specification = WindowSpecification());
	};
}