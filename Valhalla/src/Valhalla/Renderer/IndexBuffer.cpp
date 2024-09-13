#include "vhpch.h"

#include "IndexBuffer.h"
#include "RendererAPI.h"

#include "Valhalla/Platform/DirectX12/DirectX12IndexBuffer.h"

namespace Valhalla
{
	namespace Heimdall
	{
		Ref<IndexBuffer> IndexBuffer::Create(uint16_t size)
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

		Ref<IndexBuffer> IndexBuffer::Create(void* buffer, uint16_t size)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::DirectX11:	return nullptr;
				case RendererAPIType::DirectX12:	return Ref<Yggdrasil::DirectX12IndexBuffer>::Create(buffer, size);
				case RendererAPIType::Vulkan:		return nullptr;
			}

			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}
}