module Valhalla.Renderer.SwapChain;

import Valhalla.Renderer.RendererAPI;
import Valhalla.Core.Ref;
import Valhalla.Core.Assert;

import Valhalla.Yggdrasil.DirectX12.SwapChain;

namespace Valhalla
{
	namespace Heimdall
	{
		Ref<SwapChain> SwapChain::Create()
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::DirectX11:	return nullptr;
				case RendererAPIType::DirectX12:	return Ref<Yggdrasil::DirectX12SwapChain>::Create();
				case RendererAPIType::Vulkan:		return nullptr;
			}

			Assert(false, "Unknown Renderer API");
			return nullptr;
		}
	}
}