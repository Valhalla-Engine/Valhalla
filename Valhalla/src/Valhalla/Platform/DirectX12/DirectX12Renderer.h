#pragma once
#include "VD3D12.h"
#include "Valhalla/Renderer/RendererAPI.h"

#include "DirectX12Context.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		struct DirectX12DescriptorHeaps
		{
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RTVHeap;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> SRVHeap;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CBVHeap;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> UAVHeap;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> SamplerHeap;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DSVHeap;
		};

		class DirectX12Renderer : public Heimdall::RendererAPI
		{
		public:

			~DirectX12Renderer() = default;
			virtual void Init() override;
			virtual void Shutdown() override;

			virtual void BeginFrame() override;
			virtual void EndFrame() override;

			static std::vector<DirectX12DescriptorHeaps>& GetHeaps();
			static DirectX12DescriptorHeaps& GetFrameHeaps(uint32_t frameIndex);

			void WaitForFrame();


		private:
			Microsoft::WRL::ComPtr<ID3D12RootSignature> m_RootSignature;
			Microsoft::WRL::ComPtr<ID3D12Resource> m_RenderTargets[2];

			std::vector<Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>> m_RTVHeaps;

			Ref<DirectX12Context> m_Context;

			CD3DX12_VIEWPORT m_viewport;
			CD3DX12_RECT m_ScissorRect;

			uint32_t m_FrameIndex;
			HANDLE m_fenceEvent;
			Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;
			uint64_t m_FenceValue;

			inline static DirectX12Renderer* instance;
		};
	}
}