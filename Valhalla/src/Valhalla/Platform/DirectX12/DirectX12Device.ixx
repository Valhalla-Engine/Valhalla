module;

#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <atomic>
#include <vector>
#include <map>
#include <thread>
#include <unordered_map>
#include <string>


export module Valhalla.Yggdrasil.DirectX12.Device;

import Valhalla.Core.Ref;
import Valhalla.Yggdrasil.DirectX12.VD3D12;


export namespace Valhalla
{
	export namespace Yggdrasil
	{
		enum class AllocatorType {
			Compute, Copy, Bundle
		};

		class DirectX12PhysicalDevice : public RefCounted
		{
		public:
			DirectX12PhysicalDevice();
			~DirectX12PhysicalDevice();

			Microsoft::WRL::ComPtr<IDXGIAdapter4> GetAdapter() const { return m_Adapter; }

			const DXGI_ADAPTER_DESC3& GetProperties() const { return m_Properties; }

			DXGI_FORMAT GetDepthFormat() const { return m_DepthFormat; }

			static Ref<DirectX12PhysicalDevice> Select();

		private:
			DXGI_FORMAT FindDepthFormat() const;
			static const char* FeatureLevelToString(D3D_FEATURE_LEVEL level)
			{
				switch (level)
				{
				case D3D_FEATURE_LEVEL_12_2: return "12_2";
				case D3D_FEATURE_LEVEL_12_1: return "12_1";
				case D3D_FEATURE_LEVEL_12_0: return "12_0";
				case D3D_FEATURE_LEVEL_11_1: return "11_1";
				case D3D_FEATURE_LEVEL_11_0: return "11_0";
				default: return "Unknown";
				}
			}

		private:
			Microsoft::WRL::ComPtr<IDXGIAdapter4> m_Adapter;

			DXGI_ADAPTER_DESC3 m_Properties = {};

			DXGI_FORMAT m_DepthFormat = DXGI_FORMAT_UNKNOWN;

			friend class DirectX12Device;
		};

		class DirectX12CommandPool : public RefCounted
		{
		public:
			struct CommandBuffer
			{
				Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> CommandList;
				Microsoft::WRL::ComPtr<ID3D12CommandAllocator> Allocator;

				D3D12_COMMAND_LIST_TYPE Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			};

		public:
			DirectX12CommandPool();
			virtual ~DirectX12CommandPool();

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> AllocateCommandBuffer(bool begin, bool compute = false);

			void FlushCommandBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7>& commandBuffer);
			void FlushCommandBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7>& commandBuffer, Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue);
		private:
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetAvailableAllocator(D3D12_COMMAND_LIST_TYPE type);



		private:
			Microsoft::WRL::ComPtr<ID3D12Device> m_Device;
			std::unordered_map<ID3D12GraphicsCommandList7*, CommandBuffer> m_CommandLists;

			std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> m_AvailableGraphicsAllocators;
			std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> m_AvailableComputeAllocators;
			Microsoft::WRL::ComPtr<ID3D12Fence> m_Fence;
			HANDLE m_FenceEvent = nullptr;

			uint64_t m_FenceValue = 0;
		};

		class DirectX12Device : public RefCounted
		{
		public:
			DirectX12Device();
			virtual ~DirectX12Device();

			const Microsoft::WRL::ComPtr<ID3D12Device>& GetDevice() const { return m_Device; }

			const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetGraphicsQueue() const { return m_GraphicsQueue; }
			const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetComputeQueue() const { return m_ComputeQueue; }

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> GetCommandBuffer(bool begin, bool compute = false);

			void FlushCommandBuffer(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> commandBuffer);
			void FlushCommandBuffer(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> commandBuffer, Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue);

			const Ref<DirectX12PhysicalDevice>& GetPhysicalDevice() const { return m_PhysicalDevice; }

		private:
			Ref<DirectX12CommandPool> GetThreadLocalCommandPool();
			Ref<DirectX12CommandPool> GetOrCreateThreadLocalCommandPool();
		private:
			Microsoft::WRL::ComPtr<ID3D12Device> m_Device;

			Ref<DirectX12PhysicalDevice> m_PhysicalDevice;

			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_GraphicsQueue;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_ComputeQueue;

			std::map<std::thread::id, Ref<DirectX12CommandPool>> m_CommandPools;
		};
	}
}