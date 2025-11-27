export module Valhalla.Imgui.Layer;

import Valhalla.Core.Layer;

export namespace Valhalla
{
	class ImGuiLayer : public Layer
	{
	public:
		virtual void Begin() = 0;
		virtual void End() = 0;

		void SetDarkThemeColors();
		void SetDarkTheme2VColors();

		void AllowInputEvents(bool allowEvents);

		static ImGuiLayer* Create();
	};
}