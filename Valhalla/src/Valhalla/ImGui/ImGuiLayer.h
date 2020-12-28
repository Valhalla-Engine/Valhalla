#pragma once

#include "Valhalla/Core/Layer.h"

namespace Valhalla
{
	class ImGuiLayer : public Layer
	{
	public:
		virtual void Begin() = 0;
		virtual void End() = 0;

		static ImGuiLayer* Create();
	};
}