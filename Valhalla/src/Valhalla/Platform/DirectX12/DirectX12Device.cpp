#include "vhpch.h"
#include "DirectX12Device.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
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
		}
	}
}