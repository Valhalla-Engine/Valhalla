#include "vhpch.h"
#include "DirectX12RenderCommandBuffer.h"

#include "DirectX12Context.h"


namespace Valhalla
{
	namespace Yggdrasil
	{
		DirectX12RenderCommandBuffer::DirectX12RenderCommandBuffer(uint32_t count, std::string debugName)
			: m_DebugName(std::move(debugName))
		{
			auto device = DirectX12Context::GetCurrentDevice()->GetDevice();
			uint32_t framesInFlight = Heimdall::Renderer::GetConfig().FramesInFlight;

			m_CommandAllocators.resize(framesInFlight);
			for (size_t i = 0; i < m_CommandAllocators.size(); i++)
			{
				DXCALL(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
					IID_PPV_ARGS(m_CommandAllocators[i].GetAddressOf())));
				m_CommandAllocators[i]->Reset();
#if VH_DEBUG
				std::wstring temp = std::wstring(debugName.begin(), debugName.end());
				m_CommandAllocators[i]->SetName(temp.c_str());
#endif
			}

			if (count == 0)
				count = framesInFlight;
			m_CommandBuffers.resize(count);
			for (size_t i = 0; i < m_CommandBuffers.size(); i++)
			{
				DXCALL(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, 
					m_CommandAllocators[i].Get(), nullptr, IID_PPV_ARGS(m_CommandBuffers[i].GetAddressOf())));
			}

			m_Fences.resize(count);
			for (size_t i = 0; i < m_Fences.size(); i++)
			{
				DXCALL(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_Fences[i].Fence.GetAddressOf())));
				m_Fences[i].FenceValue++;
			}
		}

		DirectX12RenderCommandBuffer::DirectX12RenderCommandBuffer(std::string debugName)
			: m_DebugName(std::move(debugName))
		{
			auto device = DirectX12Context::GetCurrentDevice()->GetDevice();
			uint32_t framesInFlight = Heimdall::Renderer::GetConfig().FramesInFlight;
			m_CommandAllocators.resize(framesInFlight);
			for (size_t i = 0; i < m_CommandAllocators.size(); i++)
			{
				DXCALL(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
					IID_PPV_ARGS(m_CommandAllocators[i].GetAddressOf())));
				m_CommandAllocators[i]->Reset();
#if VH_DEBUG
				std::wstring temp = std::wstring(debugName.begin(), debugName.end());
				m_CommandAllocators[i]->SetName(temp.c_str());
#endif
			}

			m_CommandBuffers.resize(framesInFlight);
			for (size_t i = 0; i < m_CommandBuffers.size(); i++)
			{
				DXCALL(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
					m_CommandAllocators[i].Get(), nullptr, IID_PPV_ARGS(m_CommandBuffers[i].GetAddressOf())));
				m_CommandBuffers[i]->Close();
			}

			m_Fences.resize(framesInFlight);
			for (size_t i = 0; i < m_Fences.size(); i++)
			{
				DXCALL(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_Fences[i].Fence.GetAddressOf())));
				m_Fences[i].FenceValue++;
			}
		}

		void DirectX12RenderCommandBuffer::Begin()
		{
			Ref<DirectX12RenderCommandBuffer> instance = this;

			Heimdall::Renderer::Submit([instance]() mutable 
			{
				uint32_t frameIndex = Heimdall::Renderer::RT_GetCurrentFrameIndex();

				Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandBuffer = instance->m_CommandBuffers[frameIndex];

				instance->m_ActiveCommandBuffer = commandBuffer;

				instance->m_CommandAllocators[frameIndex]->Reset();
				instance->m_ActiveCommandBuffer->Reset(instance->m_CommandAllocators[frameIndex].Get(), nullptr);
			});
		}

		void DirectX12RenderCommandBuffer::End()
		{
			Ref<DirectX12RenderCommandBuffer> instance = this;
			Heimdall::Renderer::Submit([instance]() mutable 
			{
					instance->m_ActiveCommandBuffer->Close();
					instance->m_ActiveCommandBuffer = nullptr;
			});
		}

		void DirectX12RenderCommandBuffer::Submit()
		{
			Ref<DirectX12RenderCommandBuffer> instance = this;
			Heimdall::Renderer::Submit([instance]() mutable 
			{
				auto device = DirectX12Context::GetCurrentDevice();
				uint32_t frameIndex = Heimdall::Renderer::RT_GetCurrentFrameIndex();

				instance->WaitForFence(frameIndex);

				ID3D12CommandList* ppCommandList[] = { instance->m_CommandBuffers[frameIndex].Get()};
				device->GetGraphicsQueue()->ExecuteCommandLists(1, ppCommandList);

				instance->CreateFenceSignal(frameIndex);
			});
		}

		void DirectX12RenderCommandBuffer::CreateFenceSignal(uint32_t frameIndex)
		{
			auto device = DirectX12Context::GetCurrentDevice();

			auto& fence = m_Fences[frameIndex];
			fence.FenceEvent = CreateEvent(nullptr, false, false, nullptr);
			if (fence.FenceEvent == nullptr)
			{
				HRESULT_FROM_WIN32(GetLastError());
			}

			const uint64_t fencevalue = fence.FenceValue;
			DXCALL(device->GetGraphicsQueue()->Signal(fence.Fence.Get(), fencevalue));
		}

		void DirectX12RenderCommandBuffer::WaitForFence(uint32_t frameIndex)
		{
		
			auto& fence = m_Fences[frameIndex];
			if (fence.Fence->GetCompletedValue() < fence.FenceValue)
			{
				DXCALL(fence.Fence->SetEventOnCompletion(fence.FenceValue, fence.FenceEvent));
				WaitForSingleObject(fence.FenceEvent, INFINITE);
			}
			fence.FenceValue++;
		}
	}
}