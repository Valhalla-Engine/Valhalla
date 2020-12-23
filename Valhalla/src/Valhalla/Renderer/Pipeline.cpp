#include "vhpch.h"
#include "Pipeline.h"

#include "RendererAPI.h"

namespace Valhalla
{
	namespace Heimdal
	{
		Ref<Pipeline> Pipeline::Create(const PipelineSpecification& spec)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::OpenGL:		return nullptr;
				case RendererAPIType::DirectX11:	VH_CORE_ASSERT(false, "DirectX 11 is currently not supported"); return nullptr;
				case RendererAPIType::DirectX12:	VH_CORE_ASSERT(false, "DirectX 12 is currently not supported"); return nullptr;
				case RendererAPIType::Vulkan:		VH_CORE_ASSERT(false, "Vulkan is currently not supported"); return nullptr;
				case RendererAPIType::Metal:		VH_CORE_ASSERT(false, "Metal is currently not supported"); return nullptr;
			}


			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}
}