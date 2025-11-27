#include "D12Include.hpp"

export module Valhalla.Yggdrasil.DirectX12.RenderCommandBuffer;

import Valhalla.Renderer.RenderCommandBuffer;

import Valhalla.Core.Assert;

import <vector>;

export namespace Valhalla
{
	export namespace Yggdrasil
	{
		class DirectX12RenderCommandBuffer : public Heimdall::RenderCommandBuffer
		{
		public:
			DirectX12RenderCommandBuffer(uint32_t count = 0, std::string debugName = "");
			DirectX12RenderCommandBuffer(std::string debugName = "");
			~DirectX12RenderCommandBuffer();

			virtual void Begin() override;
			virtual void End() override;
			virtual void Submit() override;


			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> GetActiveCommandBuffer() const { return m_ActiveCommandBuffer; }
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> GetCommandBuffer(uint32_t frameIndex) const
			{
				Assert(frameIndex < m_CommandBuffers.size());
				return m_CommandBuffers[frameIndex];
			}

			void WaitForFence(uint32_t frameIndex);
			void CreateFenceSignal(uint32_t frameIndex);

		private:
			struct WaitFence
			{
				Microsoft::WRL::ComPtr<ID3D12Fence> Fence;
				uint64_t FenceValue;
				HANDLE FenceEvent;
			};

			std::string m_DebugName;
			std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> m_CommandAllocators;
			std::vector<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7>> m_CommandBuffers;
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> m_ActiveCommandBuffer;
			std::vector<WaitFence> m_Fences;
		};
	}
}