#pragma once

#include "VD3D12.h"
#include "Valhalla/Core/Ref.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		class DirectX12CommandAllocator : public RefCounted
		{
		public:
			DirectX12CommandAllocator();
			~DirectX12CommandAllocator();

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> AllocateCommandList(bool begin, bool compute = false);
			void FlushCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandBuffer);
			void FlushCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandBuffer,
				Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue);

			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetGraphicsCommandAllocator() { return m_GraphicsCommandAllocator; }
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetComputeCommandAllocator() { return m_ComputeCommandAllocator; }
		private:
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_GraphicsCommandAllocator, m_ComputeCommandAllocator;
		};

		class DirectX12Device : public RefCounted
		{
		public:
			DirectX12Device();
			virtual ~DirectX12Device();

			Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() { return m_Device; }
			Microsoft::WRL::ComPtr<IDXGIDebug1> GetDebug() 
			{ 
				/*if (m_dxgiDebug)
				{
					m_dxgiDebug->AddRef();
					ULONG refCount = m_dxgiDebug->Release();
					refCount--;
					VH_CORE_INFO("Number of refs of the Debug: {0}", refCount);
				}*/
				return m_dxgiDebug; 
			}

			Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetGraphicsQueue() { return m_GraphicsQueue; }
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetComputeQueue() { return m_ComputeQueue; }

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CreateSecondaryCommandBuffer(const char* debugName);

		private:
			Ref<DirectX12CommandAllocator> GetThreadLocalCommandAllocator();
			Ref<DirectX12CommandAllocator> GetOrCreateThreadLocalCommandAllocator();
		private:
			Microsoft::WRL::ComPtr<ID3D12Debug6> m_Debug;
			Microsoft::WRL::ComPtr<IDXGIDebug1> m_dxgiDebug;
			Microsoft::WRL::ComPtr<ID3D12Device> m_Device;
			Microsoft::WRL::ComPtr<ID3D12DebugDevice> m_DebugDevice;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_GraphicsQueue;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_ComputeQueue;
			std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter1>> m_PhysicalDevices;
			std::map<std::thread::id, Ref<DirectX12CommandAllocator>> m_CommandAllocators;
		};
	}
}