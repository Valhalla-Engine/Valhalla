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

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> DirectX12CommandAllocator::AllocateCommandBuffer(bool begin, bool compute /* = false */)
		{
			auto device = DirectX12Context::GetCurrentDevice();
			auto directXDevice = device->GetDevice();
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> cmdlist;
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

			if (!begin)
				DXCALL(cmdlist->Close());

			return cmdlist;
		}

		void DirectX12CommandAllocator::FlushCommandBuffer(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> commandBuffer)
		{
			auto device = DirectX12Context::GetCurrentDevice();
			FlushCommandBuffer(commandBuffer, device->GetGraphicsQueue());
		}

		void DirectX12CommandAllocator::FlushCommandBuffer(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> commandBuffer,
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue)
		{
			auto device = DirectX12Context::GetCurrentDevice();
			auto directXDevice = device->GetDevice();

			DXCALL(commandBuffer->Close());

			Microsoft::WRL::ComPtr<ID3D12Fence> fence;
			DXCALL(directXDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

			uint64_t fenceValue = 1;

			HANDLE fenceHandle = CreateEvent(nullptr, false, false, nullptr);

			{
				static std::mutex submissionLock;
				std::scoped_lock<std::mutex> lock(submissionLock);

				ID3D12CommandList* commandlists[] = { commandBuffer.Get() };
				queue->ExecuteCommandLists(1, commandlists);

				queue->Signal(fence.Get(), fenceValue);
			}

			if (fence->GetCompletedValue() < fenceValue)
			{
				DXCALL(fence->SetEventOnCompletion(fenceValue, fenceHandle));

				WaitForSingleObject(fenceHandle, INFINITE);
			}

			VH_CORE_INFO("executing command buffer!");

			m_GraphicsCommandAllocator->Reset();
			commandBuffer->Reset(m_GraphicsCommandAllocator.Get(), nullptr);
		}


		DirectX12Device::DirectX12Device()
		{
			uint32_t factoryFlags = 0;
#if defined(VH_DEBUG)
			DXCALL(D3D12GetDebugInterface(IID_PPV_ARGS(&m_Debug)));
			m_Debug->EnableDebugLayer();
			DXGIGetDebugInterface1(0, IID_PPV_ARGS(m_dxgiDebug.GetAddressOf()));
			m_dxgiDebug->EnableLeakTrackingForThread();

			DXCALL(D3D12GetDebugInterface(IID_PPV_ARGS(&m_Debug)));
			
			factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

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

			if (m_Debug.Get() != nullptr)
			{
				Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;
				m_Device->QueryInterface(IID_PPV_ARGS(infoQueue.GetAddressOf()));
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
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
#if defined(VH_DEBUG)
			m_dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
#endif
		}

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> DirectX12Device::GetCommandBuffer(bool begin, bool compute)
		{
			return GetOrCreateThreadLocalCommandAllocator()->AllocateCommandBuffer(begin, compute);
		}

		void DirectX12Device::FlushCommandBuffer(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> commandBuffer)
		{
			return GetThreadLocalCommandAllocator()->FlushCommandBuffer(commandBuffer);
		}

		void DirectX12Device::FlushCommandBuffer(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> commandBuffer, Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue)
		{
			return GetThreadLocalCommandAllocator()->FlushCommandBuffer(commandBuffer, queue);
		}

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> DirectX12Device::CreateSecondaryCommandBuffer(const char* debugName)
		{
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> cmdBuffer;
			m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, 
				GetOrCreateThreadLocalCommandAllocator()->GetGraphicsCommandAllocator().Get(), nullptr, IID_PPV_ARGS(&cmdBuffer));
			
			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			std::wstring wideDebug = converter.from_bytes(debugName);
			//cmdBuffer->SetName(wideDebug.c_str());

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