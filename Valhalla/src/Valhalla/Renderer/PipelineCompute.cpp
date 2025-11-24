module Valhalla.Renderer.PipelineCompute;

import Valhalla.Renderer.RendererAPI;

import Valhalla.Core.Assert;


namespace Valhalla
{
	namespace Heimdall
	{
		Ref<PipelineCompute> PipelineCompute::Create(Ref<Shader> computeShader)
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