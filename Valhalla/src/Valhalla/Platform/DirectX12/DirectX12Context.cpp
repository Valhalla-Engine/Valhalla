#include "vhpch.h"
#include "DirectX12Context.h"
#include "Valhalla/Core/Application.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		DirectX12Context::DirectX12Context()
		{
		}

		DirectX12Context::~DirectX12Context()
		{
			m_SwapChain = nullptr;
			m_Device->GetDebug()->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
		}

		void DirectX12Context::Init()
		{
			m_Device = Ref<DirectX12Device>::Create();

			D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			DXCALL(m_Device->GetDevice()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue)));
			m_CommandQueue->SetName(L"Main Command Queue");

			m_SwapChain = Application::Get().GetWindow().GetSwapChain().As<DirectX12SwapChain>();
			m_SwapChain->Init();
		}

	}
}