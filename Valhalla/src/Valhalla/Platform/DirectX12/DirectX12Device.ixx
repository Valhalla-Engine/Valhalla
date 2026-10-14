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
#include <mutex>


export module Valhalla.Yggdrasil.DirectX12.Device;

import Valhalla.Core.Ref;
import Valhalla.Yggdrasil.DirectX12.VD3D12;
import Valhalla.Yggdrasil.DirectX12.FrameContext;


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
				uint32_t FrameIndex = 0;
				uint64_t FrameNumber = 0;
			};

			struct FrameAllocators
			{
				std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> Graphics;
				std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> Compute;

				uint32_t NextGraphics = 0;
				uint32_t NextCompute = 0;
			};

			struct FinishedList
			{
				Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> List;
				D3D12_COMMAND_LIST_TYPE Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
				uint32_t FrameIndex = 0;
				uint64_t FrameNumber = 0;
			};

		public:
			DirectX12CommandPool();
			virtual ~DirectX12CommandPool();

			void InitializeFrames(uint32_t frameCount);

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> AllocateCommandBuffer(uint32_t frameIndex, uint64_t frameNumber, bool begin, bool compute = false);

			FinishedList FinishCommandBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7>& commandList);

			void ResetFrameAllocators(uint32_t frameIndex);

			void FlushCommandBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7>& commandBuffer);
			void FlushCommandBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7>& commandBuffer, Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue);

			
		private:
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> AcquireAllocator(uint32_t frameIndex, D3D12_COMMAND_LIST_TYPE type);
		private:
			Microsoft::WRL::ComPtr<ID3D12Device> m_Device;
			std::unordered_map<ID3D12GraphicsCommandList7*, CommandBuffer> m_CommandLists;
			std::vector<FrameAllocators> m_FrameAllocators;

			Microsoft::WRL::ComPtr<ID3D12Fence> m_ImmediateFence;
			HANDLE m_ImmediateFenceEvent = nullptr;
			uint64_t m_ImmediateFenceValue = 0;
		};

		class DirectX12Device : public RefCounted
		{
		public:
			DirectX12Device();
			virtual ~DirectX12Device();

			const Microsoft::WRL::ComPtr<ID3D12Device>& GetDevice() const { return m_Device; }
			const Ref<DirectX12PhysicalDevice>& GetPhysicalDevice() const { return m_PhysicalDevice; }

			const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetGraphicsQueue() const { return m_GraphicsQueue; }
			const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetComputeQueue() const { return m_ComputeQueue; }

			void InitializeFrames(uint32_t frameCount);

			void BeginFrame(DirectX12FrameContext& frame);
			void EndFrame();

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> GetCommandBuffer(bool begin = true, bool compute = false);
			void FinishCommandBuffer(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7>& commandBuffer);

			// Scoped worker-job binding: the caller must ensure the frame stays
			// alive and that EndFrame waits for its worker jobs. Each thread has
			// its own binding; never rely on a shared mutable active pointer there.
			void BindRecordingFrame(uint32_t frameIndex, uint64_t frameNumber);
			void UnbindRecordingFrame();

			void FlushCommandBuffer(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> commandBuffer);
			void FlushCommandBuffer(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> commandBuffer, Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue);

			// Wait for all previously submitted work on the device queues before
			// resize/shutdown; does not finish outstanding CPU recording jobs.
			 void WaitForIdle();

		private:
			Ref<DirectX12CommandPool> GetThreadLocalCommandPool();
			Ref<DirectX12CommandPool> GetOrCreateThreadLocalCommandPool();
			void WaitForGraphicsFence(uint64_t fenceValue);
			void WaitForComputeFence(uint64_t fenceValue);
			void ResetFrameAllocators(uint32_t frameIndex);
			
		private:
			struct FinishedCommandBuffer
			{
				Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> List;
				Ref<DirectX12CommandPool> OwnerPool;
				D3D12_COMMAND_LIST_TYPE Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
				uint32_t FrameIndex = 0;
				uint64_t FrameNumber = 0;
			};

			struct FrameCommandBuffers
			{
				std::vector<FinishedCommandBuffer> Graphics;
				std::vector<FinishedCommandBuffer> Compute;

				std::vector<FinishedCommandBuffer> InFlightGraphics;
				std::vector<FinishedCommandBuffer> InFlightCompute;
			};

			void SubmitGraphics(std::vector<FinishedCommandBuffer>& commands, DirectX12FrameContext& frame);
			void SubmitCompute(std::vector<FinishedCommandBuffer>& commands, DirectX12FrameContext& frame);

			Microsoft::WRL::ComPtr<ID3D12Device> m_Device;
			Ref<DirectX12PhysicalDevice> m_PhysicalDevice;

			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_GraphicsQueue;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_ComputeQueue;

			Microsoft::WRL::ComPtr<ID3D12Fence> m_GraphicsFence;
			Microsoft::WRL::ComPtr<ID3D12Fence> m_ComputeFence;
			HANDLE m_GraphicsFenceEvent = nullptr;
			HANDLE m_ComputeFenceEvent = nullptr;
			uint64_t m_GraphicsFenceValue = 0;
			uint64_t m_ComputeFenceValue = 0;

			std::mutex m_CommandPoolsMutex;
			std::map<std::thread::id, Ref<DirectX12CommandPool>> m_CommandPools;

			std::mutex m_FinishedCommandBuffersMutex;
			std::vector<FrameCommandBuffers> m_FinishedCommandBuffers;
			uint32_t m_FrameCount = 0;

			DirectX12FrameContext* m_ActiveFrame = nullptr;

			// Implement as a static thread_local binding in Device.cpp; unlike
			// the main-thread active pointer this is job-scoped for workers.
			struct RecordingFrameBinding
			{
				DirectX12Device* Device = nullptr;
				uint32_t FrameIndex = 0;
				uint64_t FrameNumber = 0;
			};
			static thread_local RecordingFrameBinding s_RecordingFrame;
		};
	}
}