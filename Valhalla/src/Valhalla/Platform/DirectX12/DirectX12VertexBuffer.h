#pragma once

#include "VD3D12.h"

#include "Valhalla/Renderer/VertexBuffer.h"
#include "Valhalla/Platform/DirectX12/DirectX12Context.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		class DirectX12VertexBuffer : public Heimdall::VertexBuffer
		{
		public:
			DirectX12VertexBuffer(void* data, uint32_t size, uint32_t stride, Heimdall::VertexBufferUsage usage);
			DirectX12VertexBuffer(uint32_t size, uint32_t stride, Heimdall::VertexBufferUsage usage);

			virtual void SetData(void* buffer, uint32_t size, uint32_t offset = 0) override;
			virtual void RT_SetData(void* buffer, uint32_t size, uint32_t offset = 0) override;
			virtual void Bind() const override;

			virtual uint32_t GetSize() const override;
		private:
			Microsoft::WRL::ComPtr<ID3D12Resource> m_VertexBuffer;
			Microsoft::WRL::ComPtr<ID3D12Resource> m_UploadVertexBuffer;
			Heimdall::VertexBufferUsage m_Usage;
		};
	}
}