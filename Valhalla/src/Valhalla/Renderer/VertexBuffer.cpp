#include "vhpch.h"
#include "VertexBuffer.h"

#include "RendererAPI.h"


#include "Valhalla/Platform/DirectX12/DirectX12VertexBuffer.h"

namespace Valhalla
{
	namespace Heimdall
	{
		Ref<VertexBuffer> VertexBuffer::Create(void* data, uint32_t size, VertexBufferUsage usage)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::DirectX11:	return nullptr;
				case RendererAPIType::DirectX12:	return nullptr;
				case RendererAPIType::Vulkan:		return nullptr;
			}
			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}

		Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, uint32_t stride, VertexBufferUsage usage)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::DirectX11:	return nullptr;
				case RendererAPIType::DirectX12:	return nullptr;
				case RendererAPIType::Vulkan:		return nullptr;
			}
			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}
}