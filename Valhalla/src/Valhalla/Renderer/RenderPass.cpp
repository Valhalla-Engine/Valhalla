#include "vhpch.h"

#include "RenderPass.h"

#include "Valhalla/Renderer/RendererAPI.h"

namespace Valhalla
{
	namespace Heimdall
	{
		Ref<RenderPass> RenderPass::Create(const RenderPassSpecification& spec)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::DirectX12:	return nullptr;
				case RendererAPIType::Vulkan:		return nullptr;
			}

			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}
}