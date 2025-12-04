export module Valhalla.Core.Events.ApplicationEvent;

import Valhalla.Core.Events;

import <sstream>;

export namespace Valhalla
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

		virtual int32_t GetCategoryFlags() const override
		{
			return EventCategoryApplication;
		}

		static constexpr EventType GetStaticType() { return EventType::WindowResize; }

		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "WindowResize"; }
	private:
		uint32_t m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		virtual int32_t GetCategoryFlags() const override
		{
			return EventCategoryApplication;
		}

		static constexpr EventType GetStaticType() { return EventType::WindowClose; }

		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "WindowClose"; }
	};

	class AppTickEvent : public Event
	{
		AppTickEvent() = default;

		virtual int32_t GetCategoryFlags() const override
		{
			return EventCategoryApplication;
		}

		static constexpr EventType GetStaticType() { return EventType::AppTick; }

		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "AppTick"; }
	};

	class AppUpdateEvent : public Event
	{
		AppUpdateEvent() = default;

		virtual int32_t GetCategoryFlags() const override
		{
			return EventCategoryApplication;
		}

		static constexpr EventType GetStaticType() { return EventType::AppUpdate; }

		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "AppUpdate"; }
	};

	class AppRenderEvent : public Event
	{
		AppRenderEvent() = default;

		virtual int32_t GetCategoryFlags() const override
		{
			return EventCategoryApplication;
		}

		static constexpr EventType GetStaticType() { return EventType::AppRender; }

		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "AppRender"; }
	};
}