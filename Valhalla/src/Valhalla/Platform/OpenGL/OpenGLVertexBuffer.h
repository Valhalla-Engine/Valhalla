#pragma once

#include "Valhalla/Renderer/VertexBuffer.h"

#include "Valhalla/Core/Buffer.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		class OpenGLVertexBuffer : public Heimdal::VertexBuffer
		{
		public:
			OpenGLVertexBuffer(void* data, uint32_t size, Heimdal::VertexBufferUsage usage = Heimdal::VertexBufferUsage::Static);
			OpenGLVertexBuffer(uint32_t size, Heimdal::VertexBufferUsage usage = Heimdal::VertexBufferUsage::Dynamic);
			virtual ~OpenGLVertexBuffer();

			virtual void SetData(void* data, uint32_t size, uint32_t offset = 0) override;
			virtual void Bind() const override;

			virtual const Heimdal::VertexBufferLayout& GetLayout() const override { return m_Layout; }
			virtual void SetLayout(const Heimdal::VertexBufferLayout& layout) override { m_Layout = layout; }

			virtual uint32_t GetSize() const override { return m_Size; }
			virtual Heimdal::RendererID GetRendererID() const override { return m_RendererID; }
		private:
			Heimdal::RendererID m_RendererID = 0;
			uint32_t m_Size;
			Heimdal::VertexBufferUsage m_Usage;
			Heimdal::VertexBufferLayout m_Layout;

			Buffer m_LocalData;
		};
	}
}