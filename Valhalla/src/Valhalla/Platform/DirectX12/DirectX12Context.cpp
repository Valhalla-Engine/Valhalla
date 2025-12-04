module Valhalla.Yggdrasil.DirectX12.Context;


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