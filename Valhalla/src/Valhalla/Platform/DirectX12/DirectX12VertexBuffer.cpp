#include "vhpch.h"
#include "DirectX12VertexBuffer.h"

#include "Valhalla/Platform/DirectX12/DirectX12Renderer.h"

#include "Valhalla/Renderer/Vertex.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		DirectX12VertexBuffer::DirectX12VertexBuffer(void* data, uint32_t size, uint32_t stride, Heimdall::VertexBufferUsage usage)
		{
			m_Usage = usage;

			Microsoft::WRL::ComPtr<ID3D12Device> device = DirectX12Context::GetCurrentDevice()->GetDevice();
			//Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList = DirectX12Renderer::GetCommandList();

			auto heapDefault = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
			auto heapUpload = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

			CD3DX12_RESOURCE_DESC dbuffer = CD3DX12_RESOURCE_DESC::Buffer(size);

			DXCALL(device->CreateCommittedResource(
				&heapDefault,
				D3D12_HEAP_FLAG_NONE,
				&dbuffer,
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(m_VertexBuffer.GetAddressOf())));

			DXCALL(device->CreateCommittedResource(
				&heapUpload,
				D3D12_HEAP_FLAG_NONE,
				&dbuffer,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(m_UploadVertexBuffer.GetAddressOf())));

			D3D12_SUBRESOURCE_DATA srd;
			memset(&srd, 0, sizeof(D3D12_SUBRESOURCE_DATA));
			srd.pData = data;
			srd.RowPitch = size;
			srd.SlicePitch = size;

			/*cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_VertexBuffer.Get(), D3D12_RESOURCE_STATE_COMMON,
				D3D12_RESOURCE_STATE_COPY_DEST));
			UpdateSubresources<1>(cmdList.Get(), m_VertexBuffer.Get(), m_UploadVertexBuffer.Get(), 
				0, 0, 1, &srd);
			cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_VertexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_GENERIC_READ));*/

			D3D12_VERTEX_BUFFER_VIEW vbv;
			vbv.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
			vbv.StrideInBytes = stride;
			vbv.SizeInBytes = size;

			//cmdList->IASetVertexBuffers(0, 1, &vbv);
		}

		DirectX12VertexBuffer::DirectX12VertexBuffer(uint32_t size, uint32_t stride, Heimdall::VertexBufferUsage usage)
			: m_Usage(usage)
		{
			Microsoft::WRL::ComPtr<ID3D12Device> device = DirectX12Context::GetCurrentDevice()->GetDevice();
			//Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList = DirectX12Renderer::GetCommandList();

			auto heapUpload = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

			CD3DX12_RESOURCE_DESC dbuffer = CD3DX12_RESOURCE_DESC::Buffer(size);

			DXCALL(device->CreateCommittedResource(
				&heapUpload,
				D3D12_HEAP_FLAG_NONE,
				&dbuffer,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(m_UploadVertexBuffer.GetAddressOf())));
		}
	}
}