#pragma once

#include "Event.h"

#include <sstream>

namespace Valhalla
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height)
			: m_Width(width), m_Height(height) {}

		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		uint32_t m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowTitleBarHitTestEvent : public Event
	{
	public:
		WindowTitleBarHitTestEvent(int32_t x, int32_t y, int32_t& hit)
			: m_X(x), m_Y(y), m_Hit(hit) {}

		inline int32_t GetX() const { return m_X; }
		inline int32_t GetY() const { return m_Y; }
		inline void SetHit(bool hit) { m_Hit = (int)hit; }

		EVENT_CLASS_TYPE(WindowTitleBarHitTest)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		int32_t m_X;
		int32_t m_Y;
		int32_t& m_Hit;
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event
	{
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}