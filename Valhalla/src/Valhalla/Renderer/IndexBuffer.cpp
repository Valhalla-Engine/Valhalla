module Valhalla.Renderer.IndexBuffer;

import Valhalla.Core.Ref;
import Valhalla.Core.Types;
import Valhalla.Renderer.RendererAPI;
import Valhalla.Core.Assert;

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

			Assert(false, "Unknown RendererAPI");
			return nullptr;
		}

		Ref<IndexBuffer> IndexBuffer::Create(void* buffer, uint16_t size)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::DirectX11:	return nullptr;
				case RendererAPIType::DirectX12:	return nullptr;
				case RendererAPIType::Vulkan:		return nullptr;
			}

			Assert(false, "Unknown RendererAPI");
			return nullptr;
		}
	}
}