#include "vhpch.h"

#include "UniformBufferSet.h"

#include "Valhalla/Renderer/RendererAPI.h"

namespace Valhalla
{
	namespace Heimdall
	{
		Ref<UniformBufferSet> UniformBufferSet::Create(uint32_t size, uint32_t framesInFlight)
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