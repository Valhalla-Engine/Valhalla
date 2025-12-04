export module Valhalla.Core.Events;

import Valhalla.Core.Types;
import Valhalla.Core;
import <string>;
import <functional>;
import <ostream>;

namespace Valhalla
{
	export enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
		ScenePreStart, ScenePostStart, ScenePreStop, ScenePostStop,
		EditorExitPlayMode,
		SelectionChanged,
	};

	export enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4),
		EventCategoryScene			= BIT(5),
		EventCategoryEditor			= BIT(6)
	};

	export class Event
	{
	public:
		bool_t Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const int8_t* GetName() const = 0;
		virtual int32_t GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool_t IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	export class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event) {}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}

			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}