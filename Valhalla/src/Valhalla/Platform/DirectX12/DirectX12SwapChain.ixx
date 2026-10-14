module;
#include <wrl.h>
#include <vector>

#include <d3d12.h>
#include <dxgi1_4.h>

export module Valhalla.Yggdrasil.DirectX12.SwapChain;

import Valhalla.Renderer.SwapChain;
import Valhalla.Core.Assert;
import Valhalla.Core.Log;
import Valhalla.Yggdrasil.DirectX12.Device;


export namespace Valhalla
{
	export namespace Yggdrasil
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
			virtual void EndFrame() override;
			virtual void Present() override;


			virtual uint32_t GetImageCount() const override { return m_ImageCount; }

			uint32_t GetWidth() const { return m_Width; }
			uint32_t GetHeight() const { return m_Height; }

			virtual uint32_t GetCurrentBackBufferIndex() const override { return m_SwapChain->GetCurrentBackBufferIndex(); }

			DXGI_FORMAT GetColorFormat() const { return m_ColorFormat; }

			uint32_t GetCurrentBackIndex() const { return m_CurrentBufferIndex; }
			const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7>& GetCurrentDrawCommandBuffer() const
			{
				return m_CurrentCommandBuffer;
			}

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> GetDrawCommandBuffer(uint32_t index)
			{
				return m_CurrentCommandBuffer;
			}

			Microsoft::WRL::ComPtr<ID3D12Resource> GetCurrentRenderTaget()
			{
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

			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetSRVHeap()
			{
				return m_SRVHeap;
			}

			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetRTVHeap()
			{
				return m_RTVHeap;
			}

			void SetBackgroundColor(float r, float g, float b, float a);

		private:
			void FlushGPU();

		private:

			bool m_VSync = false;
			uint32_t m_Width, m_Height;

			Microsoft::WRL::ComPtr<IDXGISwapChain3> m_SwapChain = nullptr;
			uint32_t m_4xMSAAQuality = 0;
			uint32_t m_ImageCount = 1;
			uint32_t m_CurrentFrameIndex = 0;

			DXGI_FORMAT m_ColorFormat = DXGI_FORMAT_UNKNOWN;

			Ref<DirectX12Device> m_Device = nullptr;

			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_SRVHeap;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RTVHeap;

			float m_ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

			std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_RenderTargets;
			std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> m_RenderTargetViews;

			Microsoft::WRL::ComPtr<ID3D12Fence> m_FlushFence;
			uint64_t m_FlushFenceValue = 0;
			HANDLE m_FlushFenceEvent = nullptr;

			uint32_t m_CurrentBufferIndex = 0;

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> m_CurrentCommandBuffer;

			friend class DirectX12Context;
		};
	}
}