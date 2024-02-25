#include "vhpch.h"
#include "DirectX12Renderer.h"
#include "Valhalla/Core/Application.h"

#include "DirectX12Context.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		struct DirectX12RendererData
		{
			std::vector<DirectX12DescriptorHeaps> DescriptorHeaps;
		};

		static DirectX12RendererData* s_Data = nullptr;

		std::vector<DirectX12DescriptorHeaps>& DirectX12Renderer::GetHeaps()
		{
			return s_Data->DescriptorHeaps;
		}

		DirectX12DescriptorHeaps& DirectX12Renderer::GetFrameHeaps(uint32_t frameIndex)
		{
			return s_Data->DescriptorHeaps[frameIndex];
		}

		void DirectX12Renderer::Init()
		{
			s_Data = new DirectX12RendererData();

			const auto& config = Heimdall::Renderer::GetConfig();
			s_Data->DescriptorHeaps.resize(config.FramesInFlight);

			
			Heimdall::Renderer::Submit([]() mutable
			{
				auto device = DirectX12Context::GetCurrentDevice();
				auto data = DirectX12Renderer::GetHeaps();

				D3D12_DESCRIPTOR_HEAP_DESC RTVHeapDesc = {};
				RTVHeapDesc.NumDescriptors = 1000;
				RTVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				RTVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

				D3D12_DESCRIPTOR_HEAP_DESC SRVHeapDesc = {};
				SRVHeapDesc.NumDescriptors = 1000;
				SRVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				SRVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
				
				D3D12_DESCRIPTOR_HEAP_DESC CBVHeapDesc = {};
				CBVHeapDesc.NumDescriptors = 1000;
				CBVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				CBVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

				D3D12_DESCRIPTOR_HEAP_DESC UAVHeapDesc = {};
				UAVHeapDesc.NumDescriptors = 1000;
				UAVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				UAVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

				D3D12_DESCRIPTOR_HEAP_DESC SamplerHeapDesc = {};
				SamplerHeapDesc.NumDescriptors = 1000;
				SamplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
				SamplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

				D3D12_DESCRIPTOR_HEAP_DESC DSVHeapDesc = {};
				DSVHeapDesc.NumDescriptors = 1000;
				DSVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
				DSVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

				VH_CORE_INFO("Creating DescriptorHeaps");

				for (size_t i = 0; i < data.size(); i++)
				{
					DXCALL(device->GetDevice()->CreateDescriptorHeap(&RTVHeapDesc, IID_PPV_ARGS(s_Data->DescriptorHeaps[i].RTVHeap.GetAddressOf())));
					DXCALL(device->GetDevice()->CreateDescriptorHeap(&SRVHeapDesc, IID_PPV_ARGS(s_Data->DescriptorHeaps[i].SRVHeap.GetAddressOf())));
					DXCALL(device->GetDevice()->CreateDescriptorHeap(&CBVHeapDesc, IID_PPV_ARGS(s_Data->DescriptorHeaps[i].CBVHeap.GetAddressOf())));
					DXCALL(device->GetDevice()->CreateDescriptorHeap(&UAVHeapDesc, IID_PPV_ARGS(s_Data->DescriptorHeaps[i].UAVHeap.GetAddressOf())));
					DXCALL(device->GetDevice()->CreateDescriptorHeap(&SamplerHeapDesc, 
						IID_PPV_ARGS(s_Data->DescriptorHeaps[i].SamplerHeap.GetAddressOf())));
					DXCALL(device->GetDevice()->CreateDescriptorHeap(&DSVHeapDesc, IID_PPV_ARGS(s_Data->DescriptorHeaps[i].DSVHeap.GetAddressOf())));
				}
			});
		}

		void DirectX12Renderer::Shutdown()
		{

		}

		void DirectX12Renderer::BeginFrame()
		{
			s_Data->DescriptorHeaps[0].RTVHeap->SetName(L"Test");
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(s_Data->DescriptorHeaps[0].RTVHeap->GetCPUDescriptorHandleForHeapStart(), 0);
		}

		void DirectX12Renderer::EndFrame()
		{
			/*m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_RenderTargets[m_FrameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

			m_CommandList->Close();

			ID3D12CommandList* ppCommandList[] = { m_CommandList.Get() };
			m_Context->GetCurrentDevice()->GetGraphicsQueue()->ExecuteCommandLists(_countof(ppCommandList), ppCommandList);
			m_Context->GetSwapChain()->GetSwapChain()->Present(0, 0);
			WaitForFrame();*/
		}

		void DirectX12Renderer::WaitForFrame()
		{

			m_fenceEvent = CreateEvent(nullptr, false, false, nullptr);
			if (m_fenceEvent == nullptr)
			{
				HRESULT_FROM_WIN32(GetLastError());
			}

			const uint64_t fence = m_FenceValue;
			DXCALL(m_Context->GetCurrentDevice()->GetGraphicsQueue()->Signal(m_Fence.Get(), fence));+
			m_FenceValue++;

			if (m_Fence->GetCompletedValue() < fence)
			{
				DXCALL(m_Fence->SetEventOnCompletion(fence, m_fenceEvent));
				WaitForSingleObject(m_fenceEvent, INFINITE);
			}
		}

		void DirectX12Renderer::BeginRenderPass(Ref<Heimdall::RenderCommandBuffer> buffer, Ref<Heimdall::RenderPass> pass, bool explicitClear)
		{


		}

		void DirectX12Renderer::EndRenderPass(Ref<Heimdall::RenderCommandBuffer> commandBuffer)
		{

		}

		void DirectX12Renderer::SubmitFillScreenQuad(Ref<Heimdall::RenderCommandBuffer> renderCommandBuffer, Ref<Heimdall::Pipeline> pipeline, Ref<Heimdall::Shader> shader)
		{

		}

		void DirectX12Renderer::SubmitFullscreenQuadWithOverrides(Ref<Heimdall::RenderCommandBuffer> renderCommandBuffer, Ref<Heimdall::Pipeline> pipeline, Ref<Heimdall::Shader> shader, Buffer vertexShaderOverrides, Buffer fragmentShaderOverrides)
		{

		}

		std::pair<Ref<Heimdall::TextureCube>, Ref<Heimdall::TextureCube>> DirectX12Renderer::CreateEnvironmentMap(const std::string& filepath)
		{
			return {};
		}

		Ref<Heimdall::TextureCube> DirectX12Renderer::CreatePreethamSky(float turbidity, float azimuth, float inclination)
		{
			return nullptr;
		}

		void DirectX12Renderer::RenderGeometry(Ref<Heimdall::RenderCommandBuffer> renderCommandBuffer, Ref<Heimdall::Pipeline> pipeline, Ref<Heimdall::Shader> shader, Ref<Heimdall::VertexBuffer> vertexBuffer, Ref<Heimdall::IndexBuffer> indexBuffer, const DirectX::XMFLOAT4X4& transform, uint32_t indexCount)
		{

		}
	}
}