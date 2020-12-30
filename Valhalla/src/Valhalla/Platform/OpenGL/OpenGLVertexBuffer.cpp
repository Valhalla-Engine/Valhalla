#include "vhpch.h"
#include "OpenGLVertexBuffer.h"

#include <glad/glad.h>

#include "Valhalla/Renderer/Renderer.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		static GLenum OpenGLUsage(Heimdal::VertexBufferUsage usage)
		{
			switch (usage)
			{
				case Heimdal::VertexBufferUsage::Static:	return GL_STATIC_DRAW;
				case Heimdal::VertexBufferUsage::Dynamic:	return GL_DYNAMIC_DRAW;
			}
			VH_CORE_ASSERT(false, "Unknown vertex buffer usage");
			return 0;
		}

		OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, uint32_t size, Heimdal::VertexBufferUsage usage)
			: m_Size(size), m_Usage(usage)
		{
			m_LocalData = Buffer::Copy(data, size);

			Ref<OpenGLVertexBuffer> instance = this;
			Heimdal::Renderer::Submit([instance]() mutable
			{
					glCreateBuffers(1, &instance->m_RendererID);
					glNamedBufferData(instance->m_RendererID, instance->m_Size, instance->m_LocalData.Data, OpenGLUsage(instance->m_Usage));
			});
		}

		OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size, Heimdal::VertexBufferUsage usage)
			: m_Size(size), m_Usage(usage)
		{
			Ref<OpenGLVertexBuffer> instance = this;
			Heimdal::Renderer::Submit([instance]() mutable
			{
					glCreateBuffers(1, &instance->m_RendererID);
					glNamedBufferData(instance->m_RendererID, instance->m_Size, nullptr, OpenGLUsage(instance->m_Usage));
			});
		}

		OpenGLVertexBuffer::~OpenGLVertexBuffer()
		{
			GLuint rendererID = m_RendererID;
			Heimdal::Renderer::Submit([rendererID]() 
			{
					glDeleteBuffers(1, &rendererID);
			});
		}

		void OpenGLVertexBuffer::SetData(void* data, uint32_t size, uint32_t offset)
		{
			m_LocalData = Buffer::Copy(data, size);
			m_Size = size;
			Ref<OpenGLVertexBuffer> instance = this;
			Heimdal::Renderer::Submit([instance, offset]()
			{
					glNamedBufferSubData(instance->m_RendererID, offset, instance->m_Size, instance->m_LocalData.Data);
			});
		}

		void OpenGLVertexBuffer::Bind() const
		{
			Ref<const OpenGLVertexBuffer> instance = this;
			Heimdal::Renderer::Submit([instance]() 
			{
					glBindBuffer(GL_ARRAY_BUFFER, instance->m_RendererID);
			});
		}
	}
}