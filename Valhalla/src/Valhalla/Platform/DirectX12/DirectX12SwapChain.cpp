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

			DXGI_SWAP_CHAIN_DESC1 sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.BufferCount = 2;
			sd.Width = Application::Get().GetWindow().GetWidth();
			sd.Height = Application::Get().GetWindow().GetHeight();
			sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			sd.Scaling = DXGI_SCALING_STRETCH;
			sd.Stereo = FALSE;

			m_ImageCount = 2;

			{
				IDXGIFactory4* dxgiFactory = NULL;
				IDXGISwapChain1* swapChain1 = NULL;
				DXCALL(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));

				DXCALL(dxgiFactory->CreateSwapChainForHwnd(DirectX12Context::GetCurrentDevice()->GetGraphicsQueue().Get(), window, &sd, NULL, NULL, &swapChain1));
				DXCALL(swapChain1->QueryInterface(IID_PPV_ARGS(&m_SwapChain)));
				swapChain1->Release();
				dxgiFactory->Release();
				m_SwapChain->SetMaximumFrameLatency(2);
				//g_hSwapChainWaitableObject = g_pSwapChain->GetFrameLatencyWaitableObject();
			}

			ID3D12Resource* backBuffer = nullptr;
			m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
			auto desc = backBuffer->GetDesc();
			VH_CORE_INFO("backBuffer size: ({0}, {1})", desc.Width, desc.Height);

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

				m_WaitFences.resize(m_ImageCount);
				for (size_t i = 0; i < m_WaitFences.size(); i++)
				{
					DXCALL(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_WaitFences[i].GetAddressOf())));
					m_WaitFences[i]->GetCompletedValue();
#ifdef VH_DEBUG
					std::wstringstream ss;
					ss << i;
					std::wstring numStr = ss.str();
					std::wstring name = L"Fence: " + numStr;
					m_WaitFences[i]->SetName(name.c_str());
#endif
				}
				DXCALL(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_GlobalFence.GetAddressOf())));

				/*m_FenceValues.resize(m_ImageCount);
				for (auto& value : m_FenceValues)
				{
					value = 0;
				}

				m_FenceEvents.resize(m_ImageCount);
				for (size_t i = 0; i < m_FenceEvents.size(); i++)
				{
					m_FenceEvents[i] = CreateEvent(nullptr, false, false, nullptr);
				}*/
			}
		}

		void DirectX12SwapChain::Present()
		{
			DXCALL(GetCurrentDrawCommandBuffer()->Close());

			ID3D12CommandList* commandlist[] = {GetCurrentDrawCommandBuffer().Get()};
			m_Device->GetGraphicsQueue()->ExecuteCommandLists(1, commandlist);

			m_SwapChain->Present(0, 0);

			m_CurrentFence++;

			m_Device->GetGraphicsQueue()->Signal(m_GlobalFence.Get(), m_CurrentFence);

			if (m_GlobalFence->GetCompletedValue() < m_CurrentFence)
			{
				HANDLE eventHandle = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);

				m_GlobalFence->SetEventOnCompletion(m_CurrentFence, eventHandle);

				WaitForSingleObject(eventHandle, INFINITE);
				CloseHandle(eventHandle);
			}
		}

		void DirectX12SwapChain::BeginFrame()
		{
			auto device = DirectX12Context::GetCurrentDevice();
			m_CurrentBufferIndex = m_SwapChain->GetCurrentBackBufferIndex();
			DXCALL(m_CommandBuffers[0].CommandAllocator->Reset());
			DXCALL(m_CommandBuffers[0].CommandBuffer->Reset(m_CommandBuffers[m_CurrentBufferIndex].CommandAllocator.Get(), nullptr));
		}

		void DirectX12SwapChain::OnResize(uint32_t width, uint32_t height)
		{

		}

	}
}