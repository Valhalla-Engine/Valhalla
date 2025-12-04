module Valhalla.Renderer.UniformBuffer;

import Valhalla.Core.Ref;
import Valhalla.Core.Types;
import Valhalla.Core.Assert;

import Valhalla.Renderer.RendererAPI;

namespace Valhalla
{
	namespace Heimdall
	{
		Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t offset)
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