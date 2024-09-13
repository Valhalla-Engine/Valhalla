#include "vhpch.h"
#include "DirectX12Renderer.h"
#include "Valhalla/Core/Application.h"

#include "DirectX12Context.h"
#include "DirectX12RenderCommandBuffer.h"
#include "DirectX12VertexBuffer.h"
#include "DirectX12IndexBuffer.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		struct DirectX12RendererData
		{
			Ref<Heimdall::VertexBuffer> QuadVertexBuffer;
			Ref<Heimdall::IndexBuffer> QuadIndexBuffer;
		};

		static DirectX12RendererData* s_Data = nullptr;

		void DirectX12Renderer::Init()
		{
			m_DirectX12Device = DirectX12Context::GetCurrentDevice();
			m_DirectX12Swap = Application::Get().GetWindow().GetSwapChain().As<DirectX12SwapChain>();
			LoadPipeline();
			LoadAssets();
		}

		void DirectX12Renderer::Shutdown()
		{

		}

		void DirectX12Renderer::BeginFrame()
		{
			m_frameIndex = m_swapchain->GetCurrentBackBufferIndex();
			PopulateCommandList();
		}


		void DirectX12Renderer::EndFrame()
		{
			ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
			m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

			DXCALL(m_device->GetDeviceRemovedReason());

			// Present the frame.
			//DXCALL(m_swapchain->Present(1, 0));

			WaitForPreviousFrame();
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

		void DirectX12Renderer::LoadPipeline()
		{
			UINT dxgiFactoryFlags = 0;

			m_device = DirectX12Context::GetCurrentDevice()->GetDevice();
			m_commandQueue = m_DirectX12Device->GetGraphicsQueue();

			
			m_swapchain = Application::Get().GetWindow().GetSwapChain().As<DirectX12SwapChain>()->GetSwapChain();

			m_frameIndex = m_swapchain->GetCurrentBackBufferIndex();

			// Create descriptor heaps.
			{
				// Describe and create a render target view (RTV) descriptor heap.
				D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
				rtvHeapDesc.NumDescriptors = 2;
				rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				DXCALL(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RtvDescriptorHeap)));

				m_RtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			}

			// Create frame resources.
			{
				CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

				// Create a RTV for each frame.
				for (UINT n = 0; n < 2; n++)
				{
					DXCALL(m_swapchain->GetBuffer(n, IID_PPV_ARGS(&m_RenderTargets[n])));
					m_device->CreateRenderTargetView(m_RenderTargets[n].Get(), nullptr, rtvHandle);
					std::wstringstream ss;
					ss << n;
					std::wstring numStr = ss.str();
					std::wstring debugName = L"Back buffer: " + numStr;
					m_RenderTargets[n]->SetName(debugName.c_str());
					rtvHandle.Offset(1, m_RtvDescriptorSize);
				}
			}

			DXCALL(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator)));
			// Create synchronization objects and wait until assets have been uploaded to the GPU.
			{
				DXCALL(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));
				m_FenceValue = 1;

				// Create an event handle to use for frame synchronization.
				m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
				if (m_FenceEvent == nullptr)
				{
					DXCALL(HRESULT_FROM_WIN32(GetLastError()));
				}

				// Wait for the command list to execute; we are reusing the same command 
				// list in our main loop but for now, we just want to wait for setup to 
				// complete before continuing.
				WaitForPreviousFrame();
			}

		}

		void DirectX12Renderer::LoadAssets()
		{
			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
			rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
			Microsoft::WRL::ComPtr<ID3D10Blob> signature;
			DXCALL(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr));

			DXCALL(m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)));

			Ref<DirectX12Shader> shader = Heimdall::Shader::Create("Resources/Shaders/Color.hlsl", true, true).As<DirectX12Shader>();

			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
			psoDesc.InputLayout = shader->GetInputLayout();
			psoDesc.pRootSignature = m_RootSignature.Get();
			psoDesc.VS = shader->GetVertexShader();
			psoDesc.PS = shader->GetPixelShader();
			psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			psoDesc.SampleDesc.Count = 1;
			psoDesc.SampleMask = 0xffffffff;
			psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			psoDesc.NumRenderTargets = 1;

			DXCALL(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PipelineState)));
			DXCALL(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator.Get(), m_PipelineState.Get(), IID_PPV_ARGS(&m_commandList)));
			m_commandList->Close();

			Vertex vList[] = {
				{{-0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
				{{ 0.5f,-0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
				{{-0.5f,-0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
				{{-0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
				{{ 0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
				{{ 0.5f,-0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}
			};

			int vBufferSize = sizeof(vList);

			m_vBuffer = Heimdall::VertexBuffer::Create(vList, vBufferSize, Heimdall::VertexBufferUsage::Static).As<DirectX12VertexBuffer>();

			m_VertexBufferView.BufferLocation = m_vBuffer->GetVertexBuffer()->GetGPUVirtualAddress();
			m_VertexBufferView.SizeInBytes = vBufferSize;
			m_VertexBufferView.StrideInBytes = sizeof(Vertex);

			m_viewport.TopLeftX = 0.0f;
			m_viewport.TopLeftY = 0.0f;
			m_viewport.Width = static_cast<FLOAT>(Application::Get().GetWindow().GetWidth());
			m_viewport.Height = static_cast<FLOAT>(Application::Get().GetWindow().GetHeight());
			m_viewport.MinDepth = 0.0f;
			m_viewport.MaxDepth = 1.0f;

			m_scissorRect.left = 0;
			m_scissorRect.top = 0;
			m_scissorRect.right = Application::Get().GetWindow().GetWidth();
			m_scissorRect.bottom = Application::Get().GetWindow().GetHeight();
		}

		void DirectX12Renderer::PopulateCommandList()
		{

			/*D3D12_RESOURCE_DESC vDesc;
			vDesc = m_vBuffer->GetVertexBuffer()->GetDesc();*/
			 
			DXCALL(m_CommandAllocator->Reset());

			DXCALL(m_commandList->Reset(m_CommandAllocator.Get(), m_PipelineState.Get()));

			// Set necessary state.
			m_commandList->SetPipelineState(m_PipelineState.Get());
			m_commandList->SetGraphicsRootSignature(m_RootSignature.Get());
			m_commandList->RSSetViewports(1, &m_viewport);
			m_commandList->RSSetScissorRects(1, &m_scissorRect);

			Microsoft::WRL::ComPtr<ID3D12Resource> renderTarget = m_DirectX12Swap->GetCurrentRenderTaget();

			// Indicate that the back buffer will be used as a render target.
			D3D12_RESOURCE_BARRIER barrier{};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Transition.pResource = renderTarget.Get();
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

			m_commandList->ResourceBarrier(1, &barrier);

			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_DirectX12Swap->GetRTVHeap()->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_RtvDescriptorSize);
			m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);


			// Record commands.
			const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
			m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
			m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_commandList->IASetVertexBuffers(0, 1, &m_VertexBufferView);
			m_commandList->DrawInstanced(6, 1, 0, 0);

			// Indicate that the back buffer will now be used to present.
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
			m_commandList->ResourceBarrier(1, &barrier);

			DXCALL(m_commandList->Close());
		}

		void DirectX12Renderer::WaitForPreviousFrame()
		{
			// Signal and increment the fence value.
			const UINT64 fence = m_FenceValue;
			DXCALL(m_commandQueue->Signal(m_Fence.Get(), fence));
			m_FenceValue++;

			// Wait until the previous frame is finished.
			if (m_Fence->GetCompletedValue() < fence)
			{
				DXCALL(m_Fence->SetEventOnCompletion(fence, m_FenceEvent));
				WaitForSingleObject(m_FenceEvent, INFINITE);
			}

			m_frameIndex = m_swapchain->GetCurrentBackBufferIndex();
		}
	}
}