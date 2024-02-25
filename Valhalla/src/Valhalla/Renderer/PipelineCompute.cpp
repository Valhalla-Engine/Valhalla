#include "vhpch.h"
#include "PipelineCompute.h"

#include "Valhalla/Renderer/RendererAPI.h"


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
			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}
}