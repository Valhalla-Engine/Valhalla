module Valhalla.Renderer.StorageBufferSet;

import Valhalla.Renderer.RendererAPI;
import Valhalla.Core.Assert;

namespace Valhalla
{
	namespace Heimdall
	{
		Ref<StorageBufferSet> StorageBufferSet::Create(const StorageBufferSpecification& specification, uint32_t size, uint32_t framesInFlight)
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