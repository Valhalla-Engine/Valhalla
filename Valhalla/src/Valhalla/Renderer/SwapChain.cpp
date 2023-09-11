#include "vhpch.h"
#include "SwapChain.h"

#include "RendererAPI.h"

#include "Valhalla/Platform/DirectX12/DirectX12SwapChain.h"

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

			VH_CORE_ASSERT(false, "Unknown Renderer API");
			return nullptr;
		}
	}
}