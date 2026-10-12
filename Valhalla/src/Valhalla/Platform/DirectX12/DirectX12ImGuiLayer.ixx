module;
#include "D12Include.hpp"

export module Valhalla.Yggdrasil.DirectX12.ImguiLayer;

import Valhalla.Renderer.RenderCommandBuffer;

import Valhalla.Imgui.Layer;

import Valhalla.Core.Ref;

export namespace Valhalla
{
	export namespace Yggdrasil
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
			void AllocSRVHeap(D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_desc_handle);
			void FreeSRVHeap(D3D12_CPU_DESCRIPTOR_HANDLE out_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE out_gpu_desc_handle);
		
		private:

		private:
			Ref<Heimdall::RenderCommandBuffer> m_RenderCommandBuffer;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_SRVHeap;
			float_t m_Time = 0.0f;
			float m_ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
		};
	}
}