#include "vhpch.h"
#include "Image.h"


#include "Valhalla/Renderer/RendererAPI.h"

namespace Valhalla
{
	namespace Heimdall
	{
		Ref<Image2D> Image2D::Create(ImageSpecification specification, Buffer buffer)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:				return nullptr;
				case RendererAPIType::DirectX12:		return nullptr;
				case RendererAPIType::Vulkan:			return nullptr;
			}

			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}

		Ref<Image2D> Image2D::Create(ImageSpecification specification, const void* data)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:				return nullptr;
				case RendererAPIType::DirectX12:		return nullptr;
				case RendererAPIType::Vulkan:			return nullptr;
			}

			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}
}