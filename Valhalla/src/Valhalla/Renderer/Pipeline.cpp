#include "vhpch.h"
#include "Pipeline.h"

#include "Valhalla/Platform/DirectX12/DirectX12Pipeline.h"

#include "Valhalla/Renderer/RendererAPI.h"

namespace Valhalla
{
	namespace Heimdall
	{
		Ref<Pipeline> Pipeline::Create(const PipelineSpecification& specs)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::DirectX12:	return Ref<Yggdrasil::DirectX12Pipeline>::Create(specs);
				case RendererAPIType::Vulkan:		return nullptr;
			}

			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}
}