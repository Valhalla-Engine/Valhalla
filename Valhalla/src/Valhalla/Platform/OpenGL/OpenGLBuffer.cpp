#include "vhpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Valhalla
{
	namespace Yggdrasil
	{
		//////////////////////////////////////////////////////////////////////////////////
		// VertexBuffer
		//////////////////////////////////////////////////////////////////////////////////

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

		}
	}
}