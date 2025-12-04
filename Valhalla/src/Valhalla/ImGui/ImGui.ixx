#pragma once

#include <ImGui/imgui.h>

export module Valhalla.Imgui;

import Valhalla.Core.Types;

namespace Valhalla::UI
{
	static int32_t s_UIContextID = 0;
	static uint32_t s_Counter = 0;
	static int8_t s_IDBuffer[16];

	static void PushID()
	{
		ImGui::PushID(s_UIContextID++);
		s_Counter = 0;
	}

	static void PopID()
	{
		ImGui::PopID();
		s_UIContextID--;
	}

	static void BeginPropertyGrid()
	{
		PushID();
		ImGui::Columns(2);
	}
}