#pragma once

#include "Valhalla/Renderer/IndexBuffer.h"

#include "Valhalla/Core/Buffer.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		class OpenGLIndexBuffer : public Heimdal::IndexBuffer
		{
		public:
			OpenGLIndexBuffer(uint32_t size);
			OpenGLIndexBuffer(void* data, uint32_t size);
			virtual ~OpenGLIndexBuffer();

			virtual void SetData(void* data, uint32_t size, uint32_t offset = 0) override;
			virtual void Bind() const override;

			virtual uint32_t GetCount() const override { return m_Size / sizeof(uint32_t); }

			virtual uint32_t GetSize() const override { return m_Size; }
			virtual Heimdal::RendererID GetRendererID() const override { return m_RendererID; }
		private:
			Heimdal::RendererID m_RendererID = 0;
			uint32_t m_Size;

			Buffer m_LocalData;
		};
	}
}