#include "vhpch.h"
#include "DirectX12Device.h"
#include "DirectX12Context.h"

#include <locale>
#include <codecvt>

namespace Valhalla
{
	namespace Yggdrasil
	{
		DirectX12CommandAllocator::DirectX12CommandAllocator()
		{
			auto device = DirectX12Context::GetCurrentDevice();
			auto directXDevice = device->GetDevice();

			DXCALL(directXDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_GraphicsCommandAllocator)));
			DXCALL(directXDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE, IID_PPV_ARGS(&m_ComputeCommandAllocator)));

		}

		DirectX12CommandAllocator::~DirectX12CommandAllocator()
		{

		}

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> DirectX12CommandAllocator::AllocateCommandList(bool begin, bool compute /* = false */)
		{
			auto device = DirectX12Context::GetCurrentDevice();
			auto directXDevice = device->GetDevice();
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdlist;
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator;
			D3D12_COMMAND_LIST_TYPE type;

			if (compute)
			{
				allocator = m_ComputeCommandAllocator;
				type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
			}
			else
			{
				allocator = m_GraphicsCommandAllocator;
				type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			}

			DXCALL(directXDevice->CreateCommandList(0, type, allocator.Get(), nullptr, IID_PPV_ARGS(&cmdlist)));

			if (begin)
				DXCALL(cmdlist->Reset(allocator.Get(), nullptr));

			return cmdlist;
		}

		void DirectX12CommandAllocator::FlushCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandBuffer)
		{
			auto device = DirectX12Context::GetCurrentDevice();
			FlushCommandList(commandBuffer, device->GetGraphicsQueue());
		}

		void DirectX12CommandAllocator::FlushCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandBuffer,
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue)
		{
			auto device = DirectX12Context::GetCurrentDevice();
			auto directXDevice = device->GetDevice();
			Microsoft::WRL::ComPtr<ID3D12Fence> fence;


			DXCALL(commandBuffer->Close());

			uint64_t fenceValue = 0;

			DXCALL(directXDevice->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

			ID3D12CommandList* commandlists[] = {commandBuffer.Get()};
			queue->ExecuteCommandLists(1, commandlists);

			queue->Signal(fence.Get(), ++fenceValue);

			if (fence->GetCompletedValue() < fenceValue)
			{
				HANDLE fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

				VH_CORE_ASSERT(fenceEvent == nullptr);

				fence->SetEventOnCompletion(fenceValue, fenceEvent);
				WaitForSingleObject(fenceEvent, INFINITE);

				CloseHandle(fenceEvent);
			}

			fence.Reset();

			commandBuffer->Reset(m_GraphicsCommandAllocator.Get(), nullptr);
		}


		DirectX12Device::DirectX12Device()
		{
			uint32_t factoryFlags = 0;
			DXCALL(D3D12GetDebugInterface(IID_PPV_ARGS(&m_Debug)));
			m_Debug->EnableDebugLayer();
			DXGIGetDebugInterface1(0, IID_PPV_ARGS(m_dxgiDebug.GetAddressOf()));
			m_dxgiDebug->EnableLeakTrackingForThread();
			
			factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

			uint32_t i = 0;
			IDXGIAdapter1* adapter = nullptr;
			Microsoft::WRL::ComPtr<IDXGIFactory4> factory = nullptr;
			DXCALL(CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory)));
			while (factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND)
			{
				m_PhysicalDevices.push_back(adapter);
				i++;
			}

			HRESULT result = D3D12CreateDevice(m_PhysicalDevices[0].Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_Device));
			if (FAILED(result))
			{
				Microsoft::WRL::ComPtr<IDXGIAdapter> warpAdapter = nullptr;
				factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter));

				D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_Device));
			}

			factory->Release();

			D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			DXCALL(m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(m_GraphicsQueue.GetAddressOf())));
			m_GraphicsQueue->SetName(L"Graphics Command Queue");
			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
			DXCALL(m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(m_ComputeQueue.GetAddressOf())));
			m_ComputeQueue->SetName(L"Compute Command Queue");
		}

		DirectX12Device::~DirectX12Device()
		{
			m_dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
		}

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> DirectX12Device::CreateSecondaryCommandBuffer(const char* debugName)
		{
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdBuffer;
			m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, 
				GetOrCreateThreadLocalCommandAllocator()->GetGraphicsCommandAllocator().Get(), nullptr, IID_PPV_ARGS(&cmdBuffer));
			
			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			std::wstring wideDebug = converter.from_bytes(debugName);
			cmdBuffer->SetName(wideDebug.c_str());

			return cmdBuffer;
		}

		Ref<DirectX12CommandAllocator> DirectX12Device::GetThreadLocalCommandAllocator()
		{
			auto threadID = std::this_thread::get_id();
			VH_CORE_VERIFY(m_CommandAllocators.find(threadID) != m_CommandAllocators.end());
			
			return m_CommandAllocators.at(threadID);
		}

		Ref<DirectX12CommandAllocator> DirectX12Device::GetOrCreateThreadLocalCommandAllocator()
		{
			auto threadID = std::this_thread::get_id();
			auto commandPoolIt = m_CommandAllocators.find(threadID);
			if (commandPoolIt != m_CommandAllocators.end())
				return commandPoolIt->second;

			Ref<DirectX12CommandAllocator> commandPool = Ref<DirectX12CommandAllocator>::Create();
			m_CommandAllocators[threadID] = commandPool;

			return commandPool;
		}
	}
}