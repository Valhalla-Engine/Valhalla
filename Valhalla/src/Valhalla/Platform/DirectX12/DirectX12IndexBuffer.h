#pragma once

#include "Valhalla/Renderer/IndexBuffer.h"

#include "VD3D12.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		class DirectX12IndexBuffer : public Heimdall::IndexBuffer
		{
		public:
			DirectX12IndexBuffer(uint16_t size);
			DirectX12IndexBuffer(void* buffer, uint16_t size);
			virtual ~DirectX12IndexBuffer() {}

			virtual void SetData(void* data, uint16_t size, uint16_t offset) override;

			virtual uint16_t GetCount() const override { return m_Size / sizeof(uint16_t); }
			virtual uint16_t GetSize() const override { return m_Size; }

			Microsoft::WRL::ComPtr<ID3D12Resource> GetIndexBuffer() { return m_IndexBuffer; }

		private:
			Microsoft::WRL::ComPtr<ID3D12Resource> m_IndexBuffer;
			Microsoft::WRL::ComPtr<ID3D12Resource> m_UploadBuffer;
			uint16_t m_Size;
		};
	}
}