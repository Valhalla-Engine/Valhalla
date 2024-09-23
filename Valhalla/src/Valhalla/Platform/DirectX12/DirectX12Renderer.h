#pragma once
#include "VD3D12.h"
#include "Valhalla/Renderer/RendererAPI.h"

#include "DirectX12Context.h"
#include "DirectX12Pipeline.h"
#include "DirectX12Shader.h"
#include "DirectX12VertexBuffer.h"

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

			virtual void BeginRenderPass(Ref<Heimdall::RenderCommandBuffer> renderCommandBuffer, Ref<Heimdall::RenderPass> renderPass, bool explicitClear = false) override;
			virtual void EndRenderPass(Ref<Heimdall::RenderCommandBuffer> renderCommandBuffer) override;

			virtual void SubmitFillScreenQuad(Ref<Heimdall::RenderCommandBuffer> renderCommandBuffer, Ref<Heimdall::Pipeline> pipeline, Ref<Heimdall::Shader> shader) override;
			virtual void SubmitFullscreenQuadWithOverrides(Ref<Heimdall::RenderCommandBuffer> renderCommandBuffer, Ref<Heimdall::Pipeline> pipeline, Ref<Heimdall::Shader> shader, Buffer vertexShaderOverrides, Buffer fragmentShaderOverrides) override;

			virtual std::pair<Ref<Heimdall::TextureCube>, Ref<Heimdall::TextureCube>> CreateEnvironmentMap(const std::string& filepath) override;
			virtual Ref<Heimdall::TextureCube> CreatePreethamSky(float turbidity, float azimuth, float inclination) override;

			virtual void RenderGeometry(Ref<Heimdall::RenderCommandBuffer> renderCommandBuffer, Ref<Heimdall::Pipeline> pipeline, Ref<Heimdall::Shader> shader, Ref<Heimdall::VertexBuffer> vertexBuffer, Ref<Heimdall::IndexBuffer> indexBuffer, const DirectX::XMFLOAT4X4& transform, uint32_t indexCount = 0) override;
		private: 

			Ref<DirectX12Device> m_DirectX12Device;
			Ref<DirectX12SwapChain> m_DirectX12Swap;
			Ref<DirectX12VertexBuffer> m_vBuffer;
			
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