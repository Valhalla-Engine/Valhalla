#include "vhpch.h"
#include "Buffer.h"


#include "Valhalla/Platform/OpenGL/OpenGLBuffer.h"

namespace Valhalla
{
	namespace Heimdal
	{
		Ref<VertexBuffer> VertexBuffer::Create(void* data, uint32_t size, VertexBufferUsage usage)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:		return nullptr;
				case RendererAPIType::OpenGL:	return Ref<Yggdrasil::OpenGLVertexBuffer>::Create(data, size, usage);
			}

			VH_CORE_ASSERT(false, "Unknown RendererAPI ");
			return nullptr;
		}

		Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, VertexBufferUsage usage)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:		return nullptr;
				case RendererAPIType::OpenGL:	return Ref<Yggdrasil::OpenGLVertexBuffer>::Create(size, usage);
			}

			VH_CORE_ASSERT(false, "Unknown RendererAPI ");
			return nullptr;
		}

		Ref<IndexBuffer> IndexBuffer::Create(void* data, uint32_t size)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:		return nullptr;
				case RendererAPIType::OpenGL:	return Ref<Yggdrasil::OpenGLIndexBuffer>::Create(data, size);
			}

			VH_CORE_ASSERT(false, "Unknown RendererAPI ");
			return nullptr;
		}
	}
}