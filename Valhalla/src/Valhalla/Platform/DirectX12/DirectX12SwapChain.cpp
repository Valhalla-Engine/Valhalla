#include "vhpch.h"
#include "DirectX12SwapChain.h"
#include "Valhalla/Core/Application.h"

#include "Valhalla/Renderer/Renderer.h"

#include "DirectX12Context.h"

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

			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
			swapChainDesc.BufferCount = 2;
			swapChainDesc.Width = Application::Get().GetWindow().GetWidth();
			swapChainDesc.Height = Application::Get().GetWindow().GetHeight();
			swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDesc.SampleDesc.Count = 1;
			Microsoft::WRL::ComPtr<IDXGIFactory4> factory = nullptr;
			DXCALL(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&factory)));

			m_ImageCount = swapChainDesc.BufferCount;

			IDXGISwapChain1* swap;
			DXCALL(factory->CreateSwapChainForHwnd(DirectX12Context::GetCurrentDevice()->GetGraphicsQueue().Get(), window, &swapChainDesc, nullptr, nullptr, &swap));
			DXCALL(swap->QueryInterface(IID_PPV_ARGS(&m_SwapChain)));

			// Create Command buffers
			{

				m_CommandBuffers.resize(m_ImageCount);
				for (auto& commandBuffer : m_CommandBuffers)
				{
					VH_CORE_INFO("Creating draw command buffers");
					DXCALL(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandBuffer.CommandAllocator)));
					DXCALL(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandBuffer.CommandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandBuffer.CommandBuffer)));
					DXCALL(commandBuffer.CommandBuffer->Close());
				}
			}

			{
				m_WaitFences.resize(m_ImageCount);
				for (auto& fence : m_WaitFences)
				{
					DXCALL(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
				}

				m_FenceValues.resize(m_ImageCount);
				for (auto& value : m_FenceValues)
				{
					value = 0;
				}

				m_FenceEvents.resize(m_ImageCount);
			}
		}

		void DirectX12SwapChain::Present()
		{
			/*if (m_WaitFences[m_CurrentBufferIndex]->GetCompletedValue() < m_FenceValues[m_CurrentBufferIndex])
			{
				m_FenceEvents[m_CurrentBufferIndex] = CreateEvent(nullptr, FALSE, FALSE, nullptr);

				if (m_FenceEvents[m_CurrentBufferIndex] != nullptr)
				{
					m_WaitFences[m_CurrentBufferIndex]->SetEventOnCompletion(m_FenceValues[m_CurrentBufferIndex], m_FenceEvents[m_CurrentBufferIndex]);
					WaitForSingleObject(m_FenceEvents[m_CurrentBufferIndex], INFINITE);
				}
			}*/

			GetCurrentDrawCommandBuffer()->Close();

			ID3D12CommandList* commandlist[] = {GetCurrentDrawCommandBuffer().Get()};
			m_Device->GetGraphicsQueue()->ExecuteCommandLists(1, commandlist);

			//m_Device->GetGraphicsQueue()->Signal(m_WaitFences[m_CurrentBufferIndex].Get(), m_FenceValues[m_CurrentBufferIndex]);

			m_FenceValues[m_CurrentBufferIndex]++;

			m_SwapChain->Present(0, 0);
		}

		void DirectX12SwapChain::BeginFrame()
		{
			m_CommandBuffers[m_CurrentBufferIndex].CommandAllocator->Reset();
			m_CommandBuffers[m_CurrentBufferIndex].CommandBuffer->Reset(m_CommandBuffers[m_CurrentBufferIndex].CommandAllocator.Get(), nullptr);
		}

		void DirectX12SwapChain::OnResize(uint32_t width, uint32_t height)
		{

		}

	}
}