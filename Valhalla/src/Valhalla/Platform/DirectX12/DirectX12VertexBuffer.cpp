module;
#include "D12Include.hpp"
#include "DXCALL.h"

module Valhalla.Yggdrasil.DirectX12.VertexBuffer;

import Valhalla.Yggdrasil.DirectX12.Context;
import Valhalla.Yggdrasil.DirectX12.VD3D12;

import Valhalla.Core.Log;



namespace Valhalla
{
	namespace Yggdrasil
	{
		DirectX12VertexBuffer::DirectX12VertexBuffer(void* data, uint32_t size, Heimdall::VertexBufferUsage usage)
		{
			m_Usage = usage;
			m_Size = size;

			Microsoft::WRL::ComPtr<ID3D12Device> device = DirectX12Context::GetCurrentDevice()->GetDevice();
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> command = DirectX12Context::GetCurrentDevice()->GetCommandBuffer(true);

			CD3DX12_HEAP_PROPERTIES heap(D3D12_HEAP_TYPE_DEFAULT);
			CD3DX12_RESOURCE_DESC buffer = CD3DX12_RESOURCE_DESC::Buffer(m_Size);
			DXCALL(device->CreateCommittedResource(&heap, D3D12_HEAP_FLAG_NONE, &buffer, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&m_VertexBuffer)));

			m_VertexBuffer->SetName(L"Vertex Buffer Resource Heap");

			CD3DX12_HEAP_PROPERTIES uploadheap(D3D12_HEAP_TYPE_UPLOAD);
			CD3DX12_RESOURCE_DESC uploadbuffer = CD3DX12_RESOURCE_DESC::Buffer(m_Size);
			ID3D12Resource* vBufferUploadHeap;
			DXCALL(device->CreateCommittedResource(&uploadheap, D3D12_HEAP_FLAG_NONE, &uploadbuffer, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vBufferUploadHeap)));
			vBufferUploadHeap->SetName(L"Vertex Buffer Upload Resource Heap");

			D3D12_SUBRESOURCE_DATA vertexData = {};
			vertexData.pData = reinterpret_cast<BYTE*>(data);
			vertexData.RowPitch = m_Size;
			vertexData.SlicePitch = m_Size;

			D3D12_RESOURCE_BARRIER barrier;
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Transition.pResource = m_VertexBuffer.Get();
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

			command->ResourceBarrier(1, &barrier);

			UpdateSubresources(command.Get(), m_VertexBuffer.Get(), vBufferUploadHeap, 0, 0, 1, &vertexData);

			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
			command->ResourceBarrier(1, &barrier);

			DirectX12Context::GetCurrentDevice()->FlushCommandBuffer(command);
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

		void DirectX12VertexBuffer::SetData(void* buffer, uint32_t size, uint32_t offset /* = 0 */)
		{

		}

		void DirectX12VertexBuffer::RT_SetData(void* buffer, uint32_t size, uint32_t offset /* = 0 */)
		{

		}

		void DirectX12VertexBuffer::Bind() const
		{

		}

		uint32_t DirectX12VertexBuffer::GetSize() const 
		{
			return m_Size;
		}
	}
}