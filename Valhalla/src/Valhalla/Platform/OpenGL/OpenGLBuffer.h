#pragma once

#include "Valhalla/Core/Buffer.h"
#include "Valhalla/Renderer/Buffer.h"

namespace Valhalla
{
	namespace Yggdrasil
	{

		//////////////////////////////////////////////////////////////////////////////////
		// VertexBuffer
		//////////////////////////////////////////////////////////////////////////////////

		class OpenGLVertexBuffer : public Heimdal::VertexBuffer
		{
		public:
			OpenGLVertexBuffer(void* data, uint32_t size, Heimdal::VertexBufferUsage usage = Heimdal::VertexBufferUsage::Static);
			OpenGLVertexBuffer(uint32_t size, Heimdal::VertexBufferUsage usage = Heimdal::VertexBufferUsage::Dynamic);
			virtual ~OpenGLVertexBuffer();

			virtual void SetData(void* data, uint32_t size, uint32_t offset = 0) override;
			virtual void Bind() const override;

			virtual const Heimdal::BufferLayout& GetLayout() const override { return m_Layout; }
			virtual void SetLayout(const Heimdal::BufferLayout& layout) override { m_Layout = layout; }

			virtual uint32_t GetSize() const override { return m_Size; }
			virtual Heimdal::RendererID GetRendererID() const override { return m_RendererID; }

		private:
			Heimdal::RendererID m_RendererID = 0;
			uint32_t m_Size;
			Heimdal::VertexBufferUsage m_Usage;
			Heimdal::BufferLayout m_Layout;

			Buffer m_LocalData;
		};

		//////////////////////////////////////////////////////////////////////////////////
		// IndexBuffer
		//////////////////////////////////////////////////////////////////////////////////

		class OpenGLIndexBuffer : public Heimdal::IndexBuffer
		{
		public:
			OpenGLIndexBuffer(void* data, uint32_t Size);
			virtual ~OpenGLIndexBuffer();

			virtual void SetData(void* data, uint32_t size, uint32_t offset = 0) override;
			virtual void Bind() const override;

			virtual uint32_t GetCount() const override { return m_Size / sizeof(uint32_t); }

			virtual uint32_t GetSize() const override { return m_Size; }
			virtual Heimdal::RendererID GetRendererID() const { return m_RendererID; }

		private:
			Heimdal::RendererID m_RendererID = 0;
			uint32_t m_Size;

			Buffer m_LocalData;
		};
	}
}