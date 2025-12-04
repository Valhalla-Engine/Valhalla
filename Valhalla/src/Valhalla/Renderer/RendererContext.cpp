module Valhalla.Renderer.RendererContext;

import Valhalla.Renderer.RendererAPI;
import Valhalla.Core.Log;
import Valhalla.Core.Assert;

import Valhalla.Core.Ref;

import Valhalla.Yggdrasil.DirectX12.Context;

namespace Valhalla
{
	namespace Heimdall
	{
		Ref<RendererContext> RendererContext::Create()
		{
			Log::CoreInfo("Start of RendererContext Create");
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::DirectX11:	return nullptr;
				case RendererAPIType::DirectX12:	return Ref<Yggdrasil::DirectX12Context>::Create();
				case RendererAPIType::Vulkan:		return nullptr;
			}
			Log::CoreInfo("End of RendererContext Create");
			Assert(false, "Unknown Renderer API");
			return nullptr;
		}
	}
}