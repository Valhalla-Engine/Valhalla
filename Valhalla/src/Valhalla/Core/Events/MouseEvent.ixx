export module Valhalla.Core.Events.MouseEvents;

import Valhalla.Core.Events;
import Valhalla.Core.MouseCodes;

import <sstream>;

export namespace Valhalla
{
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_MouseX(x), m_MouseY(y) {}

		inline float_t GetX() const { return m_MouseX; }
		inline float_t GetY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		virtual int32_t GetCategoryFlags() const override
		{
			return EventCategoryMouse | EventCategoryInput;
		}

		static constexpr EventType GetStaticType() { return EventType::MouseMoved; }

		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "MouseMoved"; }
	private:
		float_t m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float_t xOffset, float_t yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		inline float_t GetXOffset() const { return m_XOffset; }
		inline float_t GetYOffset() const { return m_YOffset; }

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
			return ss.str();
		}

		virtual int32_t GetCategoryFlags() const override
		{
			return EventCategoryMouse | EventCategoryInput;
		}

		static constexpr EventType GetStaticType() { return EventType::MouseScrolled; }

		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "MouseScrolled"; }
	private:
		float_t m_XOffset, m_YOffset;
	};

	class MouseButtonEvent : public Event
	{
	public:
		inline MouseButton GetMouseButton() const { return m_Button; }

		virtual int32_t GetCategoryFlags() const override
		{
			return EventCategoryMouse | EventCategoryInput;
		}
	protected:
		MouseButtonEvent(MouseButton mousecode)
			: m_Button(mousecode) {}

		MouseButton m_Button;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public: 
		MouseButtonPressedEvent(MouseButton mousecode)
			: MouseButtonEvent(mousecode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		static constexpr EventType GetStaticType() { return EventType::MouseButtonPressed; }

		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "MouseButtonPressed"; }
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(MouseButton mousecode)
			: MouseButtonEvent(mousecode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		static constexpr EventType GetStaticType() { return EventType::MouseButtonReleased; }

		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "MouseButtonPressed"; }
	};
}