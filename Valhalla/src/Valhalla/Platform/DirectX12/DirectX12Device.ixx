#include <wrl.h>
#include <d3d12.h>
#include <dxgi.h>
#include <dxgidebug.h>

export module Valhalla.Yggdrasil.DirectX12.Device;

import Valhalla.Core.Ref;
import Valhalla.Yggdrasil.DirectX12.VD3D12;

import <atomic>;
import <vector>;
import <map>;
import <thread>;

export namespace Valhalla
{
	export namespace Yggdrasil
	{
		enum class AllocatorType {
			Compute, Copy, Bundle
		};

		class DirectX12ThreadContext : public RefCounted
		{
		public:
			DirectX12ThreadContext();
			~DirectX12ThreadContext();

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> AllocateComputeList();
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> AllocateCopyList();
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> AllocateBundleList();

			void ResetAllocators(size_t frameIndex);

			void AddBundleToList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> bundle);
		private:
			struct FrameAllocators
			{
				Microsoft::WRL::ComPtr<ID3D12CommandAllocator> ComputeAllocator;
				Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CopyAllocator;
				Microsoft::WRL::ComPtr<ID3D12CommandAllocator> BundleAllocator;
			};

			struct FrameBundles
			{
				std::vector<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7>> bundles;
			};

			std::vector<FrameAllocators> m_FrameAllocators;
			std::vector<FrameBundles> m_FrameBundles;
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
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetCopyQueue() {return m_CopyQueue; }

			//
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> GetCommandBuffer(bool begin, bool compute = false);

			void FlushCommandBuffer(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> commandBuffer);
			void FlushCommandBuffer(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> commandBuffer, Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue);

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> CreateSecondaryCommandBuffer(const char* debugName);

		private:
			Ref<DirectX12ThreadContext> GetThreadLocalCommandAllocator();
			Ref<DirectX12ThreadContext> GetOrCreateThreadLocalCommandAllocator();
		private:
			Microsoft::WRL::ComPtr<ID3D12Debug6> m_Debug;
#if defined(VH_DEBUG)
			Microsoft::WRL::ComPtr<IDXGIDebug1> m_dxgiDebug;
#endif
			struct FrameFenceValues
			{
				std::atomic<uint64_t> m_GraphicsFenceValue;
				std::atomic<uint64_t> m_ComputeFenceValue;
				std::atomic<uint64_t> m_CopyFenceValue;
			};

			Microsoft::WRL::ComPtr<ID3D12Device> m_Device;
			Microsoft::WRL::ComPtr<ID3D12DebugDevice> m_DebugDevice;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_GraphicsQueue;
			Microsoft::WRL::ComPtr<ID3D12Fence1> m_GraphicsFence;
			std::vector<ID3D12CommandAllocator> m_GraphicsCommandAllocator;
			std::vector<ID3D12GraphicsCommandList7> m_PrimaryCommandLists;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_ComputeQueue;
			Microsoft::WRL::ComPtr<ID3D12Fence1> m_ComputeFence;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CopyQueue;
			Microsoft::WRL::ComPtr<ID3D12Fence1> m_CopyFence;
			std::atomic<uint64_t> m_GlobalResourceCopyValue;
			std::vector<FrameFenceValues> m_FenceValues;
			std::vector<Microsoft::WRL::ComPtr<IDXGIAdapter1>> m_PhysicalDevices;
			std::map<std::thread::id, Ref<DirectX12ThreadContext>> m_CommandAllocators;
		};
	}
}