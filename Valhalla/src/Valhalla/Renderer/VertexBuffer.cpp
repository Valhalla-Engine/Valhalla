#include "vhpch.h"
#include "VertexBuffer.h"

namespace Valhalla
{
	namespace Heimdal
	{
		Ref<VertexBuffer> VertexBuffer::Create(void* data, uint32_t size, VertexBufferUsage usage)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::OpenGL:		return nullptr;
				case RendererAPIType::DirectX11:	VH_CORE_ERROR("DirectX11 not currently supported"); return nullptr;
				case RendererAPIType::DirectX12:	VH_CORE_ERROR("DirectX12 not currently supported"); return nullptr;
				case RendererAPIType::Vulkan:		VH_CORE_ERROR("Vulkan not currently supported"); return nullptr;
				case RendererAPIType::Metal:		VH_CORE_ERROR("Metal not currently supported"); return nullptr;
			}
			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}

		Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, VertexBufferUsage usage)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::OpenGL:		return nullptr;
				case RendererAPIType::DirectX11:	VH_CORE_ERROR("DirectX11 not currently supported"); return nullptr;
				case RendererAPIType::DirectX12:	VH_CORE_ERROR("DirectX12 not currently supported"); return nullptr;
				case RendererAPIType::Vulkan:		VH_CORE_ERROR("Vulkan not currently supported"); return nullptr;
				case RendererAPIType::Metal:		VH_CORE_ERROR("Metal not currently supported"); return nullptr;
			}
			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}
}