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

			m_SwapChain = Application::Get().GetWindow().GetSwapChain().As<DirectX12SwapChain>();
			m_SwapChain->Init();
		}

	}
}