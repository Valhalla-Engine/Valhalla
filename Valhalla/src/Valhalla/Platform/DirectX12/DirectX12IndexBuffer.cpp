#include "vhpch.h"

#include "DirectX12IndexBuffer.h"
#include "DirectX12Renderer.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		DirectX12IndexBuffer::DirectX12IndexBuffer(uint32_t size)
			: m_Size(size)
		{

		}

		DirectX12IndexBuffer::DirectX12IndexBuffer(void* buffer, uint32_t size)
			: m_Size(size)
		{
			Microsoft::WRL::ComPtr<ID3D12Device> device = DirectX12Context::GetCurrentDevice()->GetDevice();
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList = DirectX12Renderer::GetCommandList();

			DXCALL(device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(size),
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(m_IndexBuffer.GetAddressOf())));

			DXCALL(device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(size),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(m_UploadBuffer.GetAddressOf())));

			D3D12_SUBRESOURCE_DATA srd;
			memset(&srd, 0, sizeof(D3D12_SUBRESOURCE_DATA));
			srd.pData = buffer;
			srd.RowPitch = size;
			srd.SlicePitch = size;

			cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_IndexBuffer.Get(), D3D12_RESOURCE_STATE_COMMON,
				D3D12_RESOURCE_STATE_COPY_DEST));
			UpdateSubresources<1>(cmdList.Get(), m_IndexBuffer.Get(), m_UploadBuffer.Get(), 0, 0, 1, &srd);
			cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_IndexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_COMMON));

			D3D12_INDEX_BUFFER_VIEW ibv;
			ibv.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
			ibv.SizeInBytes = m_Size;
			ibv.Format = DXGI_FORMAT_R32_UINT;

			cmdList->IASetIndexBuffer(&ibv);
			cmdList->DrawIndexedInstanced(GetCount(), 1, 0, 0, 0);
		}

		void DirectX12IndexBuffer::SetData(void* buffer, uint32_t size, uint32_t offset)
		{

		}
	}
}