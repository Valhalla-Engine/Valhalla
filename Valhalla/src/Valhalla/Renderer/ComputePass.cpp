module Valhalla.Renderer.ComputePass;

import Valhalla.Renderer.RendererAPI;
import Valhalla.Core.Assert;
import Valhalla.Core.Ref;

namespace Valhalla
{
	namespace Heimdall
	{
		Ref<ComputePass> ComputePass::Create(const ComputePassSpecification& spec)
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