#include <wrl.h>
#include <d3d12.h>
#include <dxgi.h>
#include <dxgidebug.h>

export module Valhalla.Yggdrasil.DirectX12.Device;

import Valhalla.Core.Ref;
import Valhalla.Yggdrasil.DirectX12.VD3D12;

import <vector>;
import <map>;
import <thread>;

export namespace Valhalla
{
	export namespace Yggdrasil
	{
		class DirectX12CommandAllocator : public RefCounted
		{
		public:
			DirectX12CommandAllocator();
			~DirectX12CommandAllocator();

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> AllocateCommandBuffer(bool begin, bool compute = false);
			void FlushCommandBuffer(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> commandBuffer);
			void FlushCommandBuffer(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> commandBuffer,
				Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue);

			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetGraphicsCommandAllocator() { return m_GraphicsCommandAllocator; }
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetComputeCommandAllocator() { return m_ComputeCommandAllocator; }
		private:
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_GraphicsCommandAllocator, m_ComputeCommandAllocator;
			Microsoft::WRL::ComPtr<ID3D12Fence1> m_Fence;
			HANDLE m_FenceHandle;
			uint32_t m_FrameIndex;
			std::vector<uint32_t> m_FenceValues;
		};

		class DirectX12Device : public RefCounted
		{
		public:
			DirectX12Device();
			virtual ~DirectX12Device();

			Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() { return m_Device; }
#if defined(VH_DEBUG)
			Microsoft::WRL::ComPtr<IDXGIDebug1> GetDebug() { return m_dxgiDebug; }
#endif
			 
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetGraphicsQueue() { return m_GraphicsQueue; }
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetComputeQueue() { return m_ComputeQueue; }

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> GetCommandBuffer(bool begin, bool compute = false);

			void FlushCommandBuffer(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> commandBuffer);
			void FlushCommandBuffer(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> commandBuffer, Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue);

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> CreateSecondaryCommandBuffer(const char* debugName);

		private:
			Ref<DirectX12CommandAllocator> GetThreadLocalCommandAllocator();
			Ref<DirectX12CommandAllocator> GetOrCreateThreadLocalCommandAllocator();
		private:
			Microsoft::WRL::ComPtr<ID3D12Debug6> m_Debug;
#if defined(VH_DEBUG)
			Microsoft::WRL::ComPtr<IDXGIDebug1> m_dxgiDebug;
#endif
			Microsoft::WRL::ComPtr<ID3D12Device> m_Device;
			Microsoft::WRL::ComPtr<ID3D12DebugDevice> m_DebugDevice;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_GraphicsQueue;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_ComputeQueue;
			std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter1>> m_PhysicalDevices;
			std::map<std::thread::id, Ref<DirectX12CommandAllocator>> m_CommandAllocators;
		};
	}
}