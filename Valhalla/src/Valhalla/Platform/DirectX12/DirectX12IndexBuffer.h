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
			DirectX12IndexBuffer(uint32_t size);
			DirectX12IndexBuffer(void* buffer, uint32_t size);

			virtual void SetData(void* data, uint32_t size, uint32_t offset) override;

			virtual uint32_t GetCount() const override { return m_Size / sizeof(uint32_t); }
			virtual uint32_t GetSize() const override { return m_Size; }

		private:
			Microsoft::WRL::ComPtr<ID3D12Resource> m_IndexBuffer;
			Microsoft::WRL::ComPtr<ID3D12Resource> m_UploadBuffer;
			uint32_t m_Size;
		};
	}
}