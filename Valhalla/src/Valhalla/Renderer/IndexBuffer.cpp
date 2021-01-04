#include "vhpch.h"
#include "IndexBuffer.h"

#include "Renderer.h"

#include "Valhalla/Platform/OpenGL/OpenGLIndexBuffer.h"

namespace Valhalla
{
	namespace Heimdal
	{
		Ref<IndexBuffer> IndexBuffer::Create(uint32_t size)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:		return nullptr;
				case RendererAPIType::OpenGL:	return Ref<Yggdrasil::OpenGLIndexBuffer>::Create(size);
			}

			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}

		Ref<IndexBuffer> IndexBuffer::Create(void* data, uint32_t size)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:		return nullptr;
				case RendererAPIType::OpenGL:	return Ref<Yggdrasil::OpenGLIndexBuffer>::Create(data, size);
			}

			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}
}