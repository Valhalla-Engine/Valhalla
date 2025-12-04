module Valhalla.Renderer.Pipeline;

import Valhalla.Renderer.RendererAPI;
import Valhalla.Core.Assert;


namespace Valhalla
{
	namespace Heimdall
	{
		Ref<Pipeline> Pipeline::Create(const PipelineSpecification& specs)
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