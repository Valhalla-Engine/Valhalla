#include "vhpch.h"
#include "IndexBuffer.h"

namespace Valhalla
{
	namespace Heimdal
	{
		Ref<IndexBuffer> IndexBuffer::Create(uint32_t size)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::OpenGL:		return nullptr;
				case RendererAPIType::DirectX11:	VH_CORE_ASSERT(false, "DirectX 11 is currently not supported");  return nullptr;
				case RendererAPIType::DirectX12:	VH_CORE_ASSERT(false, "DirectX 12 is currently not supported");  return nullptr;
				case RendererAPIType::Vulkan:		VH_CORE_ASSERT(false, "Vulkan is currently not supported");  return nullptr;
				case RendererAPIType::Metal:		VH_CORE_ASSERT(false, "Metal is currently not supported");  return nullptr;
			}

			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}

		Ref<IndexBuffer> IndexBuffer::Create(void* data, uint32_t size)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::OpenGL:		return nullptr;
				case RendererAPIType::DirectX11:	VH_CORE_ASSERT(false, "DirectX 11 is currently not supported");  return nullptr;
				case RendererAPIType::DirectX12:	VH_CORE_ASSERT(false, "DirectX 12 is currently not supported");  return nullptr;
				case RendererAPIType::Vulkan:		VH_CORE_ASSERT(false, "Vulkan is currently not supported");  return nullptr;
				case RendererAPIType::Metal:		VH_CORE_ASSERT(false, "Metal is currently not supported");  return nullptr;
			}

			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}
}