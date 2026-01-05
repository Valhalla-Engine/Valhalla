#include "D12Include.hpp"
#include <Windows.h>  

module Valhalla.Yggdrasil.DirectX12.Device;

import Valhalla.Yggdrasil.DirectX12.Context;
import Valhalla.Yggdrasil.DirectX12.VD3D12;

import Valhalla.Core.Log;
import Valhalla.Core.Assert;
import Valhalla.Core.Application;

import Valhalla.Renderer.Renderer;

#include "DXCALL.h"

import <locale>;
import <codecvt>;
import <mutex>;

namespace Valhalla
{
	namespace Yggdrasil
	{
		DirectX12ThreadContext::DirectX12ThreadContext()
		{
			auto device = DirectX12Context::GetCurrentDevice();
			auto directXDevice = device->GetDevice();

			uint32_t framesInFlight = Heimdall::Renderer::GetConfig().FramesInFlight;

			for (uint32_t i = 0; i < framesInFlight; i++)
			{
				DXCALL(directXDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE, IID_PPV_ARGS(&m_FrameAllocators.at(i).ComputeAllocator)));
				DXCALL(directXDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COPY, IID_PPV_ARGS(&m_FrameAllocators.at(i).CopyAllocator)));
				DXCALL(directXDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(&m_FrameAllocators.at(i).BundleAllocator)));
			}
		}

		DirectX12ThreadContext::~DirectX12ThreadContext()
		{
		}



		DirectX12Device::DirectX12Device()
		{
			uint32_t factoryFlags = 0;
#if defined(VH_DEBUG)
			DXCALL(D3D12GetDebugInterface(IID_PPV_ARGS(&m_Debug)));
			m_Debug->EnableDebugLayer();
			DXCALL(DXGIGetDebugInterface1(0, IID_PPV_ARGS(m_dxgiDebug.GetAddressOf())));
			m_dxgiDebug->EnableLeakTrackingForThread();
			
			factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

			uint32_t i = 0;
			Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter = nullptr;
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
				warpAdapter->Release();
			}

			if (m_Debug.Get() != nullptr)
			{
				Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue;
				m_Device->QueryInterface(IID_PPV_ARGS(infoQueue.GetAddressOf()));
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
				infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
			}

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
			m_CommandAllocators.clear();

			m_GraphicsQueue.Reset();
			m_ComputeQueue.Reset();
			m_Device.Reset();
			m_PhysicalDevices.clear();

			if (m_dxgiDebug)
			{
				m_dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL,
					DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
			}
#endif

		}

		Ref<DirectX12CommandAllocator> DirectX12Device::GetThreadLocalCommandAllocator()
		{
			auto threadID = std::this_thread::get_id();
			Verify(m_CommandAllocators.find(threadID) != m_CommandAllocators.end());
			
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