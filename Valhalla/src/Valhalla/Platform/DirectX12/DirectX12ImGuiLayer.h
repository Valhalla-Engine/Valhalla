#pragma once

#include "Valhalla/ImGui/ImGuiLayer.h"
#include "VD3D12.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		class DirectX12ImGuiLayer : public ImGuiLayer
		{
		public:
			DirectX12ImGuiLayer();
			DirectX12ImGuiLayer(const std::string& name);
			virtual ~DirectX12ImGuiLayer();

			virtual void Begin() override;
			virtual void End() override;

			virtual void OnAttach() override;
			virtual void OnDetach() override;
			virtual void OnImGuiRender() override;

		private:
			float_t m_Time = 0.0f;
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_ImGuiAllocator;
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_ImGuiList;
		};
	}
}