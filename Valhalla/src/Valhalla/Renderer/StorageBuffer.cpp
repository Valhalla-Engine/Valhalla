module Valhalla.Renderer.StorageBuffer;

import Valhalla.Renderer.RendererAPI;
import Valhalla.Core.Assert;
import Valhalla.Core.Ref;

namespace Valhalla
{
	namespace Heimdall
	{
		Ref<StorageBuffer> StorageBuffer::Create(uint32_t size, const StorageBufferSpecification& specification)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::DirectX12:	return nullptr;
				case RendererAPIType::Vulkan:		return nullptr;
			}
			Assert(false, "Unknown RendererAPI");
			return nullptr;
		}
	}
}