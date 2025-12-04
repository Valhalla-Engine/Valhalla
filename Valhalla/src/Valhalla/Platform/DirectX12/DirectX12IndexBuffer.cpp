#include "vhpch.h"

#include "DirectX12IndexBuffer.h"
#include "DirectX12Renderer.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		DirectX12IndexBuffer::DirectX12IndexBuffer(uint16_t size)
			: m_Size(size)
		{

		}

		DirectX12IndexBuffer::DirectX12IndexBuffer(void* buffer, uint16_t size)
			: m_Size(size)
		{
			Microsoft::WRL::ComPtr<ID3D12Device> device = DirectX12Context::GetCurrentDevice()->GetDevice();

			auto heapDefault = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
			auto heapUpload = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

			CD3DX12_RESOURCE_DESC dbuffer = CD3DX12_RESOURCE_DESC::Buffer(size);

			DXCALL(device->CreateCommittedResource(
				&heapDefault,
				D3D12_HEAP_FLAG_NONE,
				&dbuffer,
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(m_IndexBuffer.GetAddressOf())));

			DXCALL(device->CreateCommittedResource(
				&heapUpload,
				D3D12_HEAP_FLAG_NONE,
				&dbuffer,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(m_UploadBuffer.GetAddressOf())));

			D3D12_SUBRESOURCE_DATA srd;
			memset(&srd, 0, sizeof(D3D12_SUBRESOURCE_DATA));
			srd.pData = buffer;
			srd.RowPitch = size;
			srd.SlicePitch = size;

			auto cmdList = DirectX12Context::GetCurrentDevice()->GetCommandBuffer(true);

			D3D12_RESOURCE_BARRIER barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Transition.pResource = m_IndexBuffer.Get();
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;

			cmdList->ResourceBarrier(1, &barrier);
			cmdList->CopyResource(m_IndexBuffer.Get(), m_UploadBuffer.Get());

			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_INDEX_BUFFER;
			cmdList->ResourceBarrier(1, &barrier);

			DirectX12Context::GetCurrentDevice()->FlushCommandBuffer(cmdList);
		}

		void DirectX12IndexBuffer::SetData(void* buffer, uint16_t size, uint16_t offset)
		{

		}
	}
}