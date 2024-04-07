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
		}

		void DirectX12Context::Init()
		{
			m_Device = Ref<DirectX12Device>::Create();
		}
	}
}