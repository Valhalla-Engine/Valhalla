#pragma once

#include "Valhalla/Core/Ref.h"
#include "Valhalla/Core/Events/Event.h"

namespace Valhalla
{
	class EditorPanel : public RefCounted
	{
	public:
		virtual ~EditorPanel() = default;

		virtual void OnImGuiRender(bool& isOpen) = 0;
		virtual void OnEvent(Valhalla::Event& e) = 0;
	};
}