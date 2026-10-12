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
			const auto& d3dDevice = device->GetDevice();

			DXCALL(d3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));

			m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

			Assert(m_FenceEvent != nullptr, "Failed to create fence event.");
		}

		DirectX12CommandPool::~DirectX12CommandPool()
		{
			m_CommandLists.clear();

			if (m_FenceEvent)
			{
				CloseHandle(m_FenceEvent);
				m_FenceEvent = nullptr;
			}
		}

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> DirectX12CommandPool::AllocateCommandBuffer(bool begin, bool compute)
		{
			Ref<DirectX12Device> device = DirectX12Context::GetCurrentDevice();
			const auto& d3dDevice = device->GetDevice();

			const D3D12_COMMAND_LIST_TYPE type = compute ? D3D12_COMMAND_LIST_TYPE_COMPUTE : D3D12_COMMAND_LIST_TYPE_DIRECT;

			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator = GetAvailableAllocator(type);

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> commandList;

			DXCALL(d3dDevice->CreateCommandList(0, type, allocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));

			if (!begin)
			{
				DXCALL(commandList->Close());
			}

			CommandBuffer buffer;

			buffer.CommandList = commandList;
			buffer.Allocator = allocator;
			buffer.Type = type;

			m_CommandLists.emplace(commandList.Get(), std::move(buffer));

			return commandList;
		}

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> DirectX12CommandPool::GetAvailableAllocator(D3D12_COMMAND_LIST_TYPE type)
		{
			auto& allocators = type == D3D12_COMMAND_LIST_TYPE_COMPUTE ? m_AvailableComputeAllocators : m_AvailableGraphicsAllocators;

			if (!allocators.empty())
			{
				Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator = std::move(allocators.back());

				allocators.pop_back();

				DXCALL(allocator->Reset());

				return allocator;
			}

			Ref<DirectX12Device> device = DirectX12Context::GetCurrentDevice();
			const auto& d3dDevice = device->GetDevice();

			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator;

			DXCALL(d3dDevice->CreateCommandAllocator(type, IID_PPV_ARGS(&allocator)));

			return allocator;
		}

		void DirectX12CommandPool::FlushCommandBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7>& commandList)
		{
			Assert(m_CommandLists.find(commandList.Get()) != m_CommandLists.end(), "Command list not allocated from this command pool.");

			auto it = m_CommandLists.find(commandList.Get());

			Ref<DirectX12Device> device = DirectX12Context::GetCurrentDevice();

			const CommandBuffer& data = it->second;

			if (data.Type == D3D12_COMMAND_LIST_TYPE_COMPUTE)
			{
				FlushCommandBuffer(commandList, device->GetComputeQueue());
			}
			else
			{
				FlushCommandBuffer(commandList, device->GetGraphicsQueue());
			}

			auto allocator = std::move(data.Allocator);
			const auto type = data.Type;

			m_CommandLists.erase(it);

			if (type == D3D12_COMMAND_LIST_TYPE_COMPUTE)
			{
				m_AvailableComputeAllocators.emplace_back(std::move(allocator));
			}
			else
			{
				m_AvailableGraphicsAllocators.emplace_back(std::move(allocator));
			}
		}

		void DirectX12CommandPool::FlushCommandBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7>& commandList, Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue)
		{
			Assert(commandList);
			Assert(queue);

			auto it = m_CommandLists.find(commandList.Get());

			Assert(it != m_CommandLists.end(), "Command list not allocated from this command pool.");

			CommandBuffer& data = it->second;

			DXCALL(commandList->Close());

			ID3D12CommandList* commandLists[] = { commandList.Get() };

			queue->ExecuteCommandLists(1, commandLists);

			const uint64_t fenceValue = ++m_FenceValue;

			DXCALL(queue->Signal(m_Fence.Get(), fenceValue));

			if (m_Fence->GetCompletedValue() < fenceValue)
			{
				DXCALL(m_Fence->SetEventOnCompletion(
					fenceValue,
					m_FenceEvent
				));

				WaitForSingleObject(
					m_FenceEvent,
					INFINITE
				);
			}

			auto allocator = std::move(data.Allocator);
			const auto type = data.Type;

			m_CommandLists.erase(it);

			if (type == D3D12_COMMAND_LIST_TYPE_COMPUTE)
			{
				m_AvailableComputeAllocators.emplace_back(std::move(allocator));
			}
			else
			{
				m_AvailableGraphicsAllocators.emplace_back(std::move(allocator));
			}
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
		}

		DirectX12Device::~DirectX12Device()
		{
			m_CommandPools.clear();
		}

		Ref<DirectX12CommandPool> DirectX12Device::GetThreadLocalCommandPool()
		{
			auto threadID = std::this_thread::get_id();
			Verify(m_CommandPools.find(threadID) != m_CommandPools.end());

			return m_CommandPools.at(threadID);
		}

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> DirectX12Device::GetCommandBuffer(bool begin, bool compute)
		{
			Ref<DirectX12CommandPool> commandPool = GetOrCreateThreadLocalCommandPool();
			return commandPool->AllocateCommandBuffer(begin, compute);
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


		Ref<DirectX12CommandPool> DirectX12Device::GetOrCreateThreadLocalCommandPool()
		{
			auto threadID = std::this_thread::get_id();
			auto commandPoolIt = m_CommandPools.find(threadID);
			if (commandPoolIt != m_CommandPools.end())
				return commandPoolIt->second;

			Ref<DirectX12CommandPool> commandPool = Ref<DirectX12CommandPool>::Create();
			m_CommandPools[threadID] = commandPool;

			return commandPool;
		}
	}
}