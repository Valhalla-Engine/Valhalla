#include "vhpch.h"
#include "DirectX12SwapChain.h"
#include "Valhalla/Core/Application.h"

#include "Valhalla/Renderer/Renderer.h"

#include "DirectX12Context.h"

#include <codecvt>

namespace Valhalla
{
	namespace Yggdrasil
	{

		DirectX12SwapChain::DirectX12SwapChain()
			: m_Height(0), m_Width(0)
		{

		}

		DirectX12SwapChain::~DirectX12SwapChain()
		{
		}

		void DirectX12SwapChain::Destroy()
		{

		}

		void DirectX12SwapChain::Init(Ref<Heimdall::RendererContext>& context)
		{
			m_Device = context.As<DirectX12Context>()->GetDevice();
		}

		void DirectX12SwapChain::InitSurface()
		{
			
		}

		void DirectX12SwapChain::CreateSwapchain(uint32_t* width, uint32_t* height, bool vsync)
		{
			Microsoft::WRL::ComPtr<ID3D12Device> device = DirectX12Context::GetCurrentDevice()->GetDevice();

			m_Width = *width;
			m_Height = *height;
			m_VSync = vsync;

			VH_CORE_INFO("SwapChain Size: ({0}, {1})", m_Width, m_Height);

			D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevel{};
			QualityLevel.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			QualityLevel.SampleCount = 4;
			QualityLevel.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
			QualityLevel.NumQualityLevels = 0;
			DXCALL(device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
				&QualityLevel, sizeof(QualityLevel)));

			m_4xMSAAQuality = QualityLevel.NumQualityLevels;

			m_SwapChain.Reset(); 

			HWND window = static_cast<HWND>(Application::Get().GetWindow().GetNativeWindow());

			m_ColorFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

			DXGI_SWAP_CHAIN_DESC1 sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.BufferCount = 2;
			sd.Width = m_Width;
			sd.Height = m_Height;
			sd.Format = m_ColorFormat;
			sd.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			sd.Scaling = DXGI_SCALING_STRETCH;
			sd.Stereo = FALSE;


			{
				IDXGIFactory4* dxgiFactory = NULL;
				IDXGISwapChain1* swapChain1 = NULL;
				DXCALL(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));

				DXCALL(dxgiFactory->CreateSwapChainForHwnd(DirectX12Context::GetCurrentDevice()->GetGraphicsQueue().Get(), window, &sd, NULL, NULL, &swapChain1));
				DXCALL(swapChain1->QueryInterface(IID_PPV_ARGS(&m_SwapChain)));
				swapChain1->Release();
				dxgiFactory->Release();
				m_SwapChain->SetMaximumFrameLatency(2);
			}

			DXGI_SWAP_CHAIN_DESC1 temp;
			m_SwapChain->GetDesc1(&temp);
			m_ImageCount = temp.BufferCount;

			{
				D3D12_DESCRIPTOR_HEAP_DESC desc;
				desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				desc.NumDescriptors = m_ImageCount;
				desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				desc.NodeMask = 1;

				DXCALL(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_RTVHeap)));

				size_t rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
				D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_RTVHeap->GetCPUDescriptorHandleForHeapStart();
				m_RenderTargetViews.resize(m_ImageCount);
				for (uint32_t i = 0; i < m_ImageCount; i++)
				{
					m_RenderTargetViews[i] = rtvHandle;
					rtvHandle.ptr += rtvDescriptorSize;
				}
			}

			{
				D3D12_DESCRIPTOR_HEAP_DESC desc;
				desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				desc.NumDescriptors = 1;
				desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
				desc.NodeMask = 0;

				DXCALL(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_SRVHeap)));
			}

			m_RenderTargets.resize(m_ImageCount);

			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RTVHeap->GetCPUDescriptorHandleForHeapStart());
			for (size_t i = 0; i < m_ImageCount; i++)
			{
				m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_RenderTargets[i]));
				device->CreateRenderTargetView(m_RenderTargets[i].Get(), nullptr, rtvHandle);
				std::wstringstream ss;
				ss << i;
				std::wstring numStr = ss.str();
				std::wstring debugName = L"Back buffer: " + numStr;
				m_RenderTargets[i]->SetName(debugName.c_str());
				rtvHandle.Offset(1, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
			}

			// Create Command buffers
			{
				m_CommandBuffers.resize(m_ImageCount);
				for (size_t i = 0; i < m_CommandBuffers.size(); i++)
				{
					SwapChainCommandBuffer& commandBuffer = m_CommandBuffers[i];
					VH_CORE_INFO("Creating draw command buffers");
					DXCALL(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandBuffer.CommandAllocator)));
					std::wstringstream ss;
					ss << i;
					std::wstring numStr = ss.str();
					std::wstring commandName = L"command allocator: " + numStr;
					std::wstring listName = L"Command list: " + numStr;

					DXCALL(commandBuffer.CommandAllocator->SetName(commandName.c_str()));
					DXCALL(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandBuffer.CommandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandBuffer.CommandBuffer)));
					DXCALL(commandBuffer.CommandBuffer->Close());
					DXCALL(commandBuffer.CommandBuffer->SetName(listName.c_str()));
				}
			}

			{

				/*m_WaitFences.resize(m_ImageCount);
				m_FenceValues.resize(m_ImageCount);
				for (size_t i = 0; i < m_ImageCount; i++)
				{
					DXCALL(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_WaitFences[i])));
					m_FenceValues[i] = 0;
					VH_CORE_INFO("Number of Fences: {0}", i);
				}

				m_FenceEvent = CreateEvent(nullptr, false, false, nullptr);*/
			}
		}

		void DirectX12SwapChain::BeginFrame()
		{
			//auto device = DirectX12Context::GetCurrentDevice();
			m_CurrentBufferIndex = m_SwapChain->GetCurrentBackBufferIndex();
			/*DXCALL(m_CommandBuffers[m_CurrentBufferIndex].CommandAllocator->Reset());
			DXCALL(m_CommandBuffers[m_CurrentBufferIndex].CommandBuffer->Reset(m_CommandBuffers[m_CurrentBufferIndex].CommandAllocator.Get(), nullptr));*/

			/*D3D12_RESOURCE_BARRIER barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = GetCurrentRenderTaget().Get();
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

			GetCurrentDrawCommandBuffer()->ResourceBarrier(1, &barrier);

			const float color[4] = {1.0f, 0.2f, 0.2f, 1.0f};
			GetCurrentDrawCommandBuffer()->ClearRenderTargetView(m_RenderTargetViews[m_CurrentBufferIndex], color, 0, nullptr);
			GetCurrentDrawCommandBuffer()->OMSetRenderTargets(1, &m_RenderTargetViews[m_CurrentBufferIndex], false, nullptr);

			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
			GetCurrentDrawCommandBuffer()->ResourceBarrier(1, &barrier);*/
		}

		void DirectX12SwapChain::Present()
		{
			/*DXCALL(GetCurrentDrawCommandBuffer()->Close());
			WaitForLastSubmittedFrame();

			ID3D12CommandList* commandlist[] = { GetCurrentDrawCommandBuffer().Get() };
			m_Device->GetGraphicsQueue()->ExecuteCommandLists(1, commandlist);

			DXCALL(m_Device->GetGraphicsQueue()->Signal(m_WaitFences[m_CurrentBufferIndex].Get(), m_FenceValues[m_CurrentBufferIndex]));*/
			DXCALL(m_SwapChain->Present(1, 0));
		}


		void DirectX12SwapChain::WaitForLastSubmittedFrame()
		{
			int64_t value = m_WaitFences[m_CurrentBufferIndex]->GetCompletedValue();
			int64_t test = m_FenceValues[m_CurrentBufferIndex];

			if (value < m_FenceValues[m_CurrentBufferIndex])
			{
				DXCALL(m_WaitFences[m_CurrentBufferIndex]->SetEventOnCompletion(m_FenceValues[m_CurrentBufferIndex], m_FenceEvent));
				WaitForSingleObject(m_FenceEvent, INFINITE);
			}
			m_FenceValues[m_CurrentBufferIndex]++;
		}

		void DirectX12SwapChain::OnResize(uint32_t width, uint32_t height)
		{
			WaitForLastSubmittedFrame();

			for (uint32_t i = 0; i < m_RenderTargets.size(); i++)
			{
				m_RenderTargets[i].Reset();
			}


			DXCALL(m_SwapChain->ResizeBuffers(2, width, height, m_ColorFormat, DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT));

			for (size_t i = 0; i < m_ImageCount; i++)
			{
				m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_RenderTargets[i]));
 				m_Device->GetDevice()->CreateRenderTargetView(m_RenderTargets[i].Get(), nullptr, m_RenderTargetViews[i]);
			}
		}

	}
}