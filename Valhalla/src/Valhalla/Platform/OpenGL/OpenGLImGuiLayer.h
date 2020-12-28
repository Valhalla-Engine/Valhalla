#pragma once
#include "Valhalla/ImGui/ImGuiLayer.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		class OpenGLImGuiLayer : public ImGuiLayer
		{
			OpenGLImGuiLayer();
			OpenGLImGuiLayer(const std::string& name);
			virtual ~OpenGLImGuiLayer();

			virtual void Begin() override;
			virtual void End() override;

			virtual void OnAttach() override;
			virtual void OnDetach() override;
			virtual void OnImGuiRender() override;
		private:
			float_t m_Time = 0.0f;
		};
	}
}