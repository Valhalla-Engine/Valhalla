#include "vhpch.h"

#include "RendererAPI.h"
#include "RendererContext.h"

#include "Valhalla/Platform/DirectX12/DirectX12Context.h"

namespace Valhalla
{
	namespace Heimdall
	{
		Ref<RendererContext> RendererContext::Create()
		{
			VH_CORE_INFO("Start of RendererContext Create");
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::DirectX11:	return nullptr;
				case RendererAPIType::DirectX12:	return Ref<Yggdrasil::DirectX12Context>::Create();
				case RendererAPIType::Vulkan:		return nullptr;
			}
			VH_CORE_INFO("End of RendererContext Create");
			VH_CORE_ASSERT(false, "Unknown Renderer API");
			return nullptr;
		}
	}
}