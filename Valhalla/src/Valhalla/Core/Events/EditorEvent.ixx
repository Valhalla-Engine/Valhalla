export module Valhalla.Core.Events.EditorEvent;

import Valhalla.Core.Events;
import <sstream>;

export namespace Valhalla
{
	class EditorExitPlayModeEvent : public Event
	{
		EditorExitPlayModeEvent() = default;

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "EditorExitPlayModeEvent";
			return ss.str();
		}

		virtual int32_t GetCategoryFlags() const override
		{
			return EventCategoryEditor;
		}

		static constexpr EventType GetStaticType() { return EventType::EditorExitPlayMode; }

		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "EditorExitPlayMod"; }
	};
}