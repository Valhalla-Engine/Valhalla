#include "vhpch.h"
#include "ComputePass.h"

#include "Valhalla/Renderer/RendererAPI.h"

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

			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}
}