#include "vhpch.h"
#include "DirectX12SwapChain.h"
#include "Valhalla/Core/Application.h"

#include "Valhalla/Renderer/Renderer.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx12.h"

#include "DirectX12Context.h"

#include <codecvt>

namespace Valhalla
{
	namespace Yggdrasil
	{

		DirectX12SwapChain::DirectX12SwapChain()
			: m_Height(0), m_Width(0), m_FenceEvent(nullptr-)
		{

		}

		DirectX12SwapChain::~DirectX12SwapChain()
		{
			Destroy();
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
				D3D12_DESCRIPTOR_HEAP_DESC desc = {};
				desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				desc.NumDescriptors = m_ImageCount;
				desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				desc.NodeMask = 1;
				DXCALL(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_RTVHeap)));

				SIZE_T rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
				D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_RTVHeap->GetCPUDescriptorHandleForHeapStart();
				m_RenderTargetViews.resize(m_ImageCount);
				for (UINT i = 0; i < m_ImageCount; i++)
				{
					m_RenderTargetViews[i] = rtvHandle;
					rtvHandle.ptr += rtvDescriptorSize;
				}
			}

			{
				D3D12_DESCRIPTOR_HEAP_DESC desc;
				desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				desc.NumDescriptors = m_ImageCount;
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

			m_Fences.resize(m_ImageCount);
			for (uint32_t i = 0; i < 2; i++)
			{
				DXCALL(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fences[i].Fence)));
				m_Fences[i].FenceValue = 0;
			}

			m_FenceEvent = CreateEvent(nullptr, false, false, nullptr);
			VH_CORE_ASSERT(m_FenceEvent, "Failed to create fence event");
		}

		void DirectX12SwapChain::BeginFrame()
		{
			m_CurrentBufferIndex = GetCurrentBackBufferIndex();
			m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % m_ImageCount;
			WaitForFrame();
			DXCALL(m_CommandBuffers[m_CurrentBufferIndex].CommandAllocator->Reset());
			DXCALL(m_CommandBuffers[m_CurrentBufferIndex].CommandBuffer->Reset(m_CommandBuffers[m_CurrentBufferIndex].CommandAllocator.Get(), nullptr));

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> list = GetCurrentDrawCommandBuffer();

			D3D12_RESOURCE_BARRIER barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Transition.pResource = GetCurrentRenderTaget().Get();
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
			list->ResourceBarrier(1, &barrier);

			auto renderTargetView = GetCurrentRenderTagetView();

			const float clear_color_with_alpha[4] = { m_ClearColor[0] * m_ClearColor[3], m_ClearColor[1] * m_ClearColor[3], m_ClearColor[2] * m_ClearColor[3], m_ClearColor[3] };
			list->ClearRenderTargetView(renderTargetView, clear_color_with_alpha, 0, nullptr);
			list->OMSetRenderTargets(1, &renderTargetView, false, nullptr);
			list->SetDescriptorHeaps(1, m_SRVHeap.GetAddressOf());


			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
			GetCurrentDrawCommandBuffer()->ResourceBarrier(1, &barrier);
		}

		void DirectX12SwapChain::Present()
		{
			UINT backbufferindex = m_SwapChain->GetCurrentBackBufferIndex();

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> list = GetCurrentDrawCommandBuffer();

			DXCALL(list->Close());

			ID3D12CommandList* commandlist[] = { list.Get() };
			m_Device->GetGraphicsQueue()->ExecuteCommandLists(1, commandlist);

			
			DXCALL(m_SwapChain->Present(1, 0));

			SyncFence& sync = m_Fences[m_CurrentFrameIndex];
			sync.FenceValue++;
			DXCALL(m_Device->GetGraphicsQueue()->Signal(sync.Fence.Get(), sync.FenceValue));
			DXCALL(m_Device->GetDevice()->GetDeviceRemovedReason());
		}

		void DirectX12SwapChain::SetBackgroundColor(float r, float g, float b, float a)
		{
			m_ClearColor[0] = r;
			m_ClearColor[1] = g;
			m_ClearColor[2] = b;
			m_ClearColor[3] = a;
			VH_CORE_INFO("Color: ({0}, {1}, {2}, {3})", r, g, b, a);
			VH_CORE_INFO("ClearColor: ({0}, {1}, {2}, {3})", m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
		}

		void DirectX12SwapChain::WaitForFrame()
		{
			SyncFence& sync = m_Fences[m_CurrentFrameIndex];
			uint64_t test = sync.Fence->GetCompletedValue();
			// Wait only if the GPU hasn't finished this frame
			if (test < sync.FenceValue)
			{
				DXCALL(sync.Fence->SetEventOnCompletion(sync.FenceValue, m_FenceEvent));
				WaitForSingleObject(m_FenceEvent, INFINITE);
			}
		}

		void DirectX12SwapChain::FlushGPU()
		{
			for (auto& sync : m_Fences)
			{
				if (sync.Fence->GetCompletedValue() < sync.FenceValue)
				{
					DXCALL(sync.Fence->SetEventOnCompletion(sync.FenceValue, m_FenceEvent));
					WaitForSingleObject(m_FenceEvent, INFINITE);
				}
			}
		}

		void DirectX12SwapChain::OnResize(uint32_t width, uint32_t height)
		{
			FlushGPU();

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