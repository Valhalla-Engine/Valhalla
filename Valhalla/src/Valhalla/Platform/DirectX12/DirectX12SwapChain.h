#pragma once
#include "VD3D12.h"
#include "DirectX12Device.h"


#include "Valhalla/Renderer/SwapChain.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		class DirectX12SwapChain : public Heimdall::SwapChain
		{
		public:
			DirectX12SwapChain();
			virtual ~DirectX12SwapChain();

			virtual void Init(Ref<Heimdall::RendererContext>& context) override;
			virtual void InitSurface() override;
			virtual void CreateSwapchain(uint32_t* width, uint32_t* height, bool vsync) override;
			virtual void Destroy() override;

			virtual void OnResize(uint32_t width, uint32_t height) override;

			virtual void BeginFrame() override;
			virtual void Present() override;
			

			virtual uint32_t GetImageCount() const override{ return m_ImageCount; }

			uint32_t GetWidth() const { return m_Width; }
			uint32_t GetHeight() const { return m_Height; }

			virtual uint32_t GetCurrentBackBufferIndex() const override { return m_SwapChain->GetCurrentBackBufferIndex(); }

			DXGI_FORMAT GetColorFormat() const { return m_ColorFormat; }

			uint32_t GetCurrentBackIndex() const { return m_CurrentBufferIndex; }
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> GetCurrentDrawCommandBuffer() 
			{ 
				//VH_CORE_INFO("Current back buffer index:{0}", m_CurrentBufferIndex);
				return GetDrawCommandBuffer(m_CurrentBufferIndex); 
			}

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> GetDrawCommandBuffer(uint32_t index)
			{
				VH_CORE_ASSERT(index < m_CommandBuffers.size());
				return m_CommandBuffers[index].CommandBuffer;
			}

			Microsoft::WRL::ComPtr<ID3D12Resource> GetCurrentRenderTaget()
			{
				VH_CORE_INFO("Current back buffer index is: {}", m_SwapChain->GetCurrentBackBufferIndex());
				return m_RenderTargets[m_CurrentBufferIndex];
			}

			D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentRenderTagetView()
			{
				return m_RenderTargetViews[m_CurrentBufferIndex];
			}

			void SetVSync(const bool enabled) { m_VSync = enabled; }

			Microsoft::WRL::ComPtr<IDXGISwapChain3> GetSwapChain()
			{
				return m_SwapChain; 
			}

			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetRTVHeap()
			{
				return m_RTVHeap;
			}
		private:
			void WaitForLastSubmittedFrame();

		private:

			bool m_VSync = false;
			uint32_t m_Width, m_Height;

			Microsoft::WRL::ComPtr<IDXGISwapChain3> m_SwapChain = nullptr;
			uint32_t m_4xMSAAQuality= 0;
			uint32_t m_ImageCount = 1;

			DXGI_FORMAT m_ColorFormat = DXGI_FORMAT_UNKNOWN;

			Ref<DirectX12Device> m_Device = nullptr;

			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_SRVHeap;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RTVHeap;

			std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_RenderTargets;
			std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> m_RenderTargetViews;

			struct SwapChainCommandBuffer
			{
				Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandAllocator = nullptr;
				Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> CommandBuffer = nullptr;
			};
			std::vector<SwapChainCommandBuffer> m_CommandBuffers;

			std::vector<Microsoft::WRL::ComPtr<ID3D12Fence>> m_WaitFences;
			std::vector<int64_t> m_FenceValues;
			HANDLE m_FenceEvent;

			uint32_t m_CurrentBufferIndex = 0;

			friend class DirectX12Context;
		};
	}
}