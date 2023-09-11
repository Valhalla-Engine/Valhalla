#pragma once

#include "VD3D12.h"
#include "Valhalla/Core/Ref.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		class DirectX12Device : public RefCounted
		{
		public:
			DirectX12Device();
			virtual ~DirectX12Device();

			Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() { return m_Device; }
			Microsoft::WRL::ComPtr<IDXGIDebug1> GetDebug() { return m_dxgiDebug; }


		private:
			Microsoft::WRL::ComPtr<ID3D12Debug6> m_Debug;
			Microsoft::WRL::ComPtr<IDXGIDebug1> m_dxgiDebug;
			Microsoft::WRL::ComPtr<ID3D12Device> m_Device;
			Microsoft::WRL::ComPtr<ID3D12DebugDevice> m_DebugDevice;
			std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter1>> m_PhysicalDevices;
		};
	}
}