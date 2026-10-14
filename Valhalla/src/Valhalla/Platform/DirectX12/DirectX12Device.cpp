module;

#include "D12Include.hpp"
#include <Windows.h>
#include "DXCALL.h"

#include <locale>
#include <codecvt>
#include <mutex>

#include <dxgi1_6.h>


module Valhalla.Yggdrasil.DirectX12.Device;

import Valhalla.Yggdrasil.DirectX12.Context;
import Valhalla.Yggdrasil.DirectX12.VD3D12;

import Valhalla.Core.Log;
import Valhalla.Core.Assert;
import Valhalla.Core.Application;

import Valhalla.Renderer.Renderer;
namespace Valhalla
{
	namespace Yggdrasil
	{
		void WaitForFenceValue(ID3D12Fence* fence, HANDLE eventHandle,
			uint64_t value)
		{
			if (value == 0 || fence->GetCompletedValue() >= value)
				return;

			DXCALL(fence->SetEventOnCompletion(value, eventHandle));
			const DWORD result = WaitForSingleObject(eventHandle, INFINITE);
			Assert(result == WAIT_OBJECT_0, "GPU fence wait failed");
		}

		thread_local DirectX12Device::RecordingFrameBinding
			DirectX12Device::s_RecordingFrame{};

		DirectX12PhysicalDevice::DirectX12PhysicalDevice()
		{
			Microsoft::WRL::ComPtr<IDXGIFactory6> factory;

			uint32_t factoryFlags = 0;

#if defined(VH_DEBUG)
			Microsoft::WRL::ComPtr<ID3D12Debug6> debugController;

			DXCALL(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
			debugController->EnableDebugLayer();
			factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

			DXCALL(CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory)));

			Microsoft::WRL::ComPtr<IDXGIAdapter4> selectedAdapter;

			for (uint32_t i = 0;; i++)
			{
				Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter;

				HRESULT result = factory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter));

				if (result == DXGI_ERROR_NOT_FOUND)
				{
					break;
				}

				if (FAILED(result)) 
					continue;

				DXGI_ADAPTER_DESC3 properties{};
				result = adapter->GetDesc3(&properties);

				if (FAILED(result))
					continue;

				if (properties.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)
					continue;

				result = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_0, __uuidof(ID3D12Device), nullptr);

				if (FAILED(result))
					continue;

				selectedAdapter = adapter;
				m_Properties = properties;

				break;
			}

			Assert(selectedAdapter != nullptr, "Failed to find a suitable GPU adapter.");

			m_Adapter = selectedAdapter;
			
			std::wstring wideName = m_Properties.Description;
			std::string adapterName(wideName.begin(), wideName.end());

			Log::CoreTraceTag(
				"Renderer",
				"Selected DirectX 12 adapter: {}",
				adapterName
			);

			Log::CoreTraceTag("Renderer", "Dedicated video memory: {}", m_Properties.DedicatedVideoMemory);

			Log::CoreTraceTag("Renderer", "Dedicated system memory: {}", m_Properties.DedicatedSystemMemory);

			Log::CoreTraceTag("Renderer", "Shared system memory: {}", m_Properties.SharedSystemMemory);

			m_DepthFormat = FindDepthFormat();

			Assert(m_DepthFormat != DXGI_FORMAT_UNKNOWN, "Couldn't find a supported depth format!");
		}

		DirectX12PhysicalDevice::~DirectX12PhysicalDevice()
		{
		}

		DXGI_FORMAT DirectX12PhysicalDevice::FindDepthFormat() const
		{
			Microsoft::WRL::ComPtr<ID3D12Device> device;

			DXCALL(D3D12CreateDevice(m_Adapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device)));

			constexpr DXGI_FORMAT depthFormats[] = {
				DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
				DXGI_FORMAT_D32_FLOAT,
				DXGI_FORMAT_D24_UNORM_S8_UINT,
				DXGI_FORMAT_D16_UNORM
			};

			for (DXGI_FORMAT format: depthFormats)
			{
				D3D12_FEATURE_DATA_FORMAT_SUPPORT formatSupport{};
				formatSupport.Format = format;

				HRESULT result = device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &formatSupport, sizeof(formatSupport));

				if (FAILED(result))
					continue;

				if (formatSupport.Support1 & D3D12_FORMAT_SUPPORT1_DEPTH_STENCIL)
				{
					return format;
				}
			}
			return DXGI_FORMAT_UNKNOWN;
		}

		Ref<DirectX12PhysicalDevice> DirectX12PhysicalDevice::Select()
		{
			return Ref<DirectX12PhysicalDevice>::Create();
		}

		DirectX12CommandPool::DirectX12CommandPool()
		{
			Ref<DirectX12Device> device = DirectX12Context::GetCurrentDevice();
			m_Device = device->GetDevice();
			DXCALL(m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE,
				IID_PPV_ARGS(&m_ImmediateFence)));
			m_ImmediateFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			Assert(m_ImmediateFenceEvent != nullptr, "Failed to create immediate fence event");
		}

		DirectX12CommandPool::~DirectX12CommandPool()
		{
			m_CommandLists.clear();
			m_FrameAllocators.clear();
			if (m_ImmediateFenceEvent)
			{
				CloseHandle(m_ImmediateFenceEvent);
				m_ImmediateFenceEvent = nullptr;
			}
		}

		void DirectX12CommandPool::InitializeFrames(uint32_t frameCount)
		{
			Assert(m_CommandLists.empty(), "Cannot resize pool while command lists are open");
			m_FrameAllocators.clear();
			m_FrameAllocators.resize(frameCount);
		}

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> DirectX12CommandPool::AcquireAllocator(uint32_t frameIndex, D3D12_COMMAND_LIST_TYPE type)
		{
			auto& frame = m_FrameAllocators.at(frameIndex);
			Assert(type == D3D12_COMMAND_LIST_TYPE_DIRECT ||
				type == D3D12_COMMAND_LIST_TYPE_COMPUTE,
				"Unsupported allocator type");

			auto& allocators = (type == D3D12_COMMAND_LIST_TYPE_DIRECT) ? frame.Graphics : frame.Compute;
			auto& next = (type == D3D12_COMMAND_LIST_TYPE_DIRECT) ? frame.NextGraphics : frame.NextCompute;

			if (next == allocators.size())
			{
				Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator;
				DXCALL(m_Device->CreateCommandAllocator(
					type, IID_PPV_ARGS(&allocator)));
				allocators.push_back(std::move(allocator));
			}

			return allocators[next++];
		}

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> DirectX12CommandPool::AllocateCommandBuffer(uint32_t frameIndex, uint64_t frameNumber, bool begin, bool compute)
		{
			const auto type = compute ? D3D12_COMMAND_LIST_TYPE_COMPUTE
				: D3D12_COMMAND_LIST_TYPE_DIRECT;
			auto allocator = AcquireAllocator(frameIndex, type);

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> list;
			DXCALL(m_Device->CreateCommandList(0, type, allocator.Get(), nullptr,
				IID_PPV_ARGS(&list)));
			if (!begin)
				DXCALL(list->Close());

			CommandBuffer record{};
			record.CommandList = list;
			record.Allocator = std::move(allocator);
			record.Type = type;
			record.FrameIndex = frameIndex;
			record.FrameNumber = frameNumber;
			m_CommandLists.emplace(list.Get(), std::move(record));
			return list;
		}

		DirectX12CommandPool::FinishedList DirectX12CommandPool::FinishCommandBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7>& commandList)
		{
			auto it = m_CommandLists.find(commandList.Get());
			Assert(it != m_CommandLists.end(), "Unknown command list");
			DXCALL(commandList->Close());

			FinishedList finished{};
			finished.List = it->second.CommandList;
			finished.Type = it->second.Type;
			finished.FrameIndex = it->second.FrameIndex;
			finished.FrameNumber = it->second.FrameNumber;

			m_CommandLists.erase(it);
			return finished;
		}

		void DirectX12CommandPool::ResetFrameAllocators(uint32_t frameIndex)
		{
			auto& frame = m_FrameAllocators.at(frameIndex);

			for (auto& allocator : frame.Graphics)
			{
				DXCALL(allocator->Reset());
			}

			for (auto& allocator : frame.Compute)
			{
				DXCALL(allocator->Reset());
			}

			frame.NextGraphics = 0;
			frame.NextCompute = 0;

		}

		void DirectX12CommandPool::FlushCommandBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7>& commandList)
		{
			auto it = m_CommandLists.find(commandList.Get());
			const auto type = it->second.Type;
			Ref<DirectX12Device> device = DirectX12Context::GetCurrentDevice();
			FlushCommandBuffer(commandList, type == D3D12_COMMAND_LIST_TYPE_COMPUTE
				? device->GetComputeQueue()
				: device->GetGraphicsQueue());
		}

		void DirectX12CommandPool::FlushCommandBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7>& commandList, Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue)
		{
			auto it = m_CommandLists.find(commandList.Get());
			Assert(it != m_CommandLists.end(), "Unknown command list");
			Assert(queue != nullptr, "Invalid command queue");

			DXCALL(commandList->Close());
			ID3D12CommandList* lists[] = { commandList.Get() };
			queue->ExecuteCommandLists(1, lists);

			const uint64_t value = ++m_ImmediateFenceValue;
			DXCALL(queue->Signal(m_ImmediateFence.Get(), value));
			WaitForFenceValue(m_ImmediateFence.Get(), m_ImmediateFenceEvent, value);

			// The allocator remains in the frame pool and is safe for its next
			// reset because the immediate submission has completed.
			m_CommandLists.erase(it);
		}

		DirectX12Device::DirectX12Device()
		{
			m_PhysicalDevice = DirectX12PhysicalDevice::Select();

			Assert(m_PhysicalDevice, "Failed to select a physical device.");

			DXCALL(D3D12CreateDevice(
				m_PhysicalDevice->GetAdapter().Get(),
				D3D_FEATURE_LEVEL_12_1,
				IID_PPV_ARGS(&m_Device)
			));

			m_Device->SetName(L"Valhalla DirectX 12 Device");

			Assert(m_Device, "Failed to create D3D12 device.");

			{
				D3D12_COMMAND_QUEUE_DESC queueDesc{};
				queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
				queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
				queueDesc.Flags	= D3D12_COMMAND_QUEUE_FLAG_NONE;
				queueDesc.NodeMask = 0;

				DXCALL(m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_GraphicsQueue)));
			}

			{
				D3D12_COMMAND_QUEUE_DESC queueDesc{};
				queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
				queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
				queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
				queueDesc.NodeMask = 0;

				DXCALL(m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_ComputeQueue)));
			}

			DXCALL(m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_GraphicsFence)));
			DXCALL(m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_ComputeFence)));

			m_GraphicsFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			m_ComputeFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		}

		DirectX12Device::~DirectX12Device()
		{
			WaitForIdle();
			{
				std::scoped_lock lock(m_CommandPoolsMutex);
				m_CommandPools.clear();
			}
			if (m_GraphicsFenceEvent) CloseHandle(m_GraphicsFenceEvent);
			if (m_ComputeFenceEvent) CloseHandle(m_ComputeFenceEvent);
		}

		void DirectX12Device::InitializeFrames(uint32_t frameCount)
		{
			Assert(frameCount > 0, "Frame count must be nonzero");
			Assert(m_ActiveFrame == nullptr, "Cannot initialize during a frame");
			Assert(m_CommandPools.empty(), "InitializeFrames before recording commands");
			m_FrameCount = frameCount;
			m_FinishedCommandBuffers.clear();
			m_FinishedCommandBuffers.resize(frameCount);
		}

		Ref<DirectX12CommandPool> DirectX12Device::GetThreadLocalCommandPool()
		{
			auto threadID = std::this_thread::get_id();
			Verify(m_CommandPools.find(threadID) != m_CommandPools.end());

			return m_CommandPools.at(threadID);
		}

		void DirectX12Device::BindRecordingFrame(uint32_t index, uint64_t number)
		{
			Assert(index < m_FrameCount, "Invalid frame index");
			Assert(s_RecordingFrame.Device == nullptr, "This thread already has a recording frame");
			s_RecordingFrame = { this, index, number };
		}

		void DirectX12Device::UnbindRecordingFrame()
		{
			Assert(s_RecordingFrame.Device == this, "This thread is not bound to this device");
			s_RecordingFrame = {};
		}

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> DirectX12Device::GetCommandBuffer(bool begin, bool compute)
		{
			Assert(s_RecordingFrame.Device == this, "Bind a recording frame before allocating command lists");
			auto pool = GetOrCreateThreadLocalCommandPool();
			return pool->AllocateCommandBuffer(s_RecordingFrame.FrameIndex, s_RecordingFrame.FrameNumber, begin, compute);
		}

		void DirectX12Device::FinishCommandBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7>& commandBuffer)
		{
			Assert(s_RecordingFrame.Device == this, "Finish a command list on its recording thread");
			auto pool = GetThreadLocalCommandPool();
			auto finished = pool->FinishCommandBuffer(commandBuffer);
			Assert(finished.FrameIndex == s_RecordingFrame.FrameIndex && finished.FrameNumber == s_RecordingFrame.FrameNumber, "Command list belongs to another frame");

			FinishedCommandBuffer entry{};
			entry.List = std::move(finished.List);
			entry.OwnerPool = pool;
			entry.Type = finished.Type;
			entry.FrameIndex = finished.FrameIndex;
			entry.FrameNumber = finished.FrameNumber;

			std::scoped_lock lock(m_FinishedCommandBuffersMutex);
			auto& target = m_FinishedCommandBuffers.at(entry.FrameIndex);
			if (entry.Type == D3D12_COMMAND_LIST_TYPE_COMPUTE)
				target.Compute.push_back(std::move(entry));
			else
				target.Graphics.push_back(std::move(entry));
		}

		void DirectX12Device::FlushCommandBuffer(Microsoft::WRL::ComPtr < ID3D12GraphicsCommandList7> commandBuffer)
		{
			Ref<DirectX12CommandPool> commandPool = GetThreadLocalCommandPool();
			commandPool->FlushCommandBuffer(commandBuffer);
		}

		void DirectX12Device::FlushCommandBuffer(Microsoft::WRL::ComPtr < ID3D12GraphicsCommandList7> commandBuffer, Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue)
		{
			Ref<DirectX12CommandPool> commandPool = GetThreadLocalCommandPool();
			commandPool->FlushCommandBuffer(commandBuffer, queue);
		}

		void DirectX12Device::ResetFrameAllocators(uint32_t index)
		{
			// Only after all jobs have joined and both queue fences for this slot
			// have completed. Do not record on pools concurrently with this loop.
			std::scoped_lock lock(m_CommandPoolsMutex);
			for (auto& [threadId, pool] : m_CommandPools)
				pool->ResetFrameAllocators(index);
		}


		void DirectX12Device::BeginFrame(DirectX12FrameContext& frame)
		{
			Assert(m_ActiveFrame == nullptr, "A frame is already active");
			Assert(frame.FrameIndex < m_FrameCount, "Invalid frame slot");

			// These waits allow multiple slots to be in flight concurrently.
			WaitForGraphicsFence(frame.GraphicsFenceValue);
			WaitForComputeFence(frame.ComputeFenceValue);
			ResetFrameAllocators(frame.FrameIndex);

			// Lists from the slot's prior use are no longer needed.
			auto& slot = m_FinishedCommandBuffers.at(frame.FrameIndex);
			Assert(slot.Graphics.empty() && slot.Compute.empty(), "Unsubmitted work remains for a frame slot");
			slot.InFlightGraphics.clear();
			slot.InFlightCompute.clear();

			m_ActiveFrame = &frame;
			BindRecordingFrame(frame.FrameIndex, frame.FrameNumber);
		}

		void DirectX12Device::SubmitGraphics( std::vector<FinishedCommandBuffer>& commands, DirectX12FrameContext& frame)
		{
			if (commands.empty()) return;
			std::vector<ID3D12CommandList*> raw;
			raw.reserve(commands.size());
			auto& slot = m_FinishedCommandBuffers.at(frame.FrameIndex);
			for (auto& command : commands)
			{
				Assert(command.FrameNumber == frame.FrameNumber, "Stale graphics command list");
				raw.push_back(command.List.Get());
				slot.InFlightGraphics.push_back(std::move(command));
			}

			m_GraphicsQueue->ExecuteCommandLists(static_cast<UINT>(raw.size()), raw.data());
			const uint64_t value = ++m_GraphicsFenceValue;
			DXCALL(m_GraphicsQueue->Signal(m_GraphicsFence.Get(), value));
			frame.GraphicsFenceValue = value;
		}

		void DirectX12Device::SubmitCompute(std::vector<FinishedCommandBuffer>& commands, DirectX12FrameContext& frame)
		{
			if (commands.empty()) return;
			std::vector<ID3D12CommandList*> raw;
			raw.reserve(commands.size());
			auto& slot = m_FinishedCommandBuffers.at(frame.FrameIndex);
			for (auto& command : commands)
			{
				Assert(command.FrameNumber == frame.FrameNumber,
					"Stale compute command list");
				raw.push_back(command.List.Get());
				slot.InFlightCompute.push_back(std::move(command));
			}

			m_ComputeQueue->ExecuteCommandLists(static_cast<UINT>(raw.size()), raw.data());
			const uint64_t value = ++m_ComputeFenceValue;
			DXCALL(m_ComputeQueue->Signal(m_ComputeFence.Get(), value));
			frame.ComputeFenceValue = value;
		}

		void DirectX12Device::EndFrame()
		{
			Assert(m_ActiveFrame != nullptr, "No active frame");
			auto& frame = *m_ActiveFrame;
			Assert(s_RecordingFrame.Device == this && s_RecordingFrame.FrameNumber == frame.FrameNumber, "Wrong recording frame on submission thread");

			
			FrameCommandBuffers completed;
			{
				std::scoped_lock lock(m_FinishedCommandBuffersMutex);
				auto& slot = m_FinishedCommandBuffers.at(frame.FrameIndex);
				completed.Graphics.swap(slot.Graphics);
				completed.Compute.swap(slot.Compute);
			}

			
			SubmitGraphics(completed.Graphics, frame);
			SubmitCompute(completed.Compute, frame);

			UnbindRecordingFrame();
			m_ActiveFrame = nullptr;
		}

		void DirectX12Device::WaitForIdle()
		{
			const uint64_t graphicsValue = ++m_GraphicsFenceValue;
			const uint64_t computeValue = ++m_ComputeFenceValue;
			DXCALL(m_GraphicsQueue->Signal(m_GraphicsFence.Get(), graphicsValue));
			DXCALL(m_ComputeQueue->Signal(m_ComputeFence.Get(), computeValue));
			WaitForGraphicsFence(graphicsValue);
			WaitForComputeFence(computeValue);
		}

		void DirectX12Device::WaitForGraphicsFence(uint64_t value)
		{
			if (value == 0)
				return;

			if (m_GraphicsFence->GetCompletedValue() >= value)
				return;

			DXCALL(m_GraphicsFence->SetEventOnCompletion(
				value,
				m_GraphicsFenceEvent
			));

			DWORD result = WaitForSingleObject(
				m_GraphicsFenceEvent,
				INFINITE
			);

			Assert(result == WAIT_OBJECT_0,
				"Failed waiting for graphics queue!");
		}

		void DirectX12Device::WaitForComputeFence(uint64_t value)
		{
			if (value == 0)
				return;

			if (m_ComputeFence->GetCompletedValue() >= value)
				return;

			DXCALL(m_ComputeFence->SetEventOnCompletion(
				value,
				m_ComputeFenceEvent
			));

			DWORD result = WaitForSingleObject(
				m_ComputeFenceEvent,
				INFINITE
			);

			Assert(result == WAIT_OBJECT_0,
				"Failed waiting for Compute queue!");
		}

		Ref<DirectX12CommandPool> DirectX12Device::GetOrCreateThreadLocalCommandPool()
		{
			std::scoped_lock lock(m_CommandPoolsMutex);
			auto id = std::this_thread::get_id();
			if (auto it = m_CommandPools.find(id); it != m_CommandPools.end())
				return it->second;

			Assert(m_FrameCount != 0, "Call InitializeFrames() first");
			auto pool = Ref<DirectX12CommandPool>::Create();
			pool->InitializeFrames(m_FrameCount);
			m_CommandPools.emplace(id, pool);
			return pool;
		}
	}
}