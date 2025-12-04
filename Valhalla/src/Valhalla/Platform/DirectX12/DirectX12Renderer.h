#pragma once
#include "VD3D12.h"
#include "Valhalla/Renderer/RendererAPI.h"

#include "DirectX12Context.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		class DirectX12Renderer : public Heimdall::RendererAPI
		{

			struct Vertex
			{
				DirectX::XMFLOAT3 position;
				DirectX::XMFLOAT4 Color;
			};


		public:

			~DirectX12Renderer() = default;
			virtual void Init() override;
			virtual void Shutdown() override;

			virtual void BeginFrame() override;
			virtual void EndFrame() override;
		private: 

			Ref<DirectX12Device> m_DirectX12Device;
			Ref<DirectX12SwapChain> m_DirectX12Swap;

			Microsoft::WRL::ComPtr<ID3D12Device> m_device;
			Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapchain;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RtvDescriptorHeap;
			Microsoft::WRL::ComPtr<ID3D12Resource> m_RenderTargets[2];
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
			Microsoft::WRL::ComPtr<ID3D12RootSignature> m_RootSignature;
			Microsoft::WRL::ComPtr<ID3D12PipelineState> m_PipelineState;
			Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;
			HANDLE m_FenceEvent;
			UINT64 m_FenceValue;
			int m_frameIndex;
			int m_RtvDescriptorSize;

			Microsoft::WRL::ComPtr<ID3D12PipelineState> m_Pipeline;
			D3D12_VIEWPORT m_viewport;
			D3D12_RECT m_scissorRect;
			Microsoft::WRL::ComPtr<ID3D12Resource> m_IndexBuffer;
			D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
			D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;

			void LoadPipeline();
			void LoadAssets();
			void PopulateCommandList();
			void WaitForPreviousFrame();

			inline static DirectX12Renderer* instance;
		};
	}
}