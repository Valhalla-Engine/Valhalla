export module Valhalla.Core.Events.KeyEvent;

import Valhalla.Core.Events;
import Valhalla.Core.KeyCodes;

import <sstream>;

export namespace Valhalla
{
	class KeyEvent : public Event
	{
	public:
		inline KeyCode getKeyCode() { return m_KeyCode; }

		virtual int32_t GetCategoryFlags() const override
		{
			return EventCategoryKeyboard | EventCategoryInput;
		}

	protected:

		KeyEvent(KeyCode keycode)
			: m_KeyCode(keycode) {}

		KeyCode m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keycode, int32_t repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int32_t GetRepeatCount() const { return m_RepeatCount; }

		static constexpr EventType GetStaticType() { return EventType::KeyPressed; }

		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "KeyPressed"; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}
	private:
		int32_t m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		static constexpr EventType GetStaticType() { return EventType::KeyReleased; }

		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "KeyReleased"; }
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(KeyCode keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		static constexpr EventType GetStaticType() { return EventType::KeyTyped; }

		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "KeyTyped"; }
	};
}