module Valhalla.Renderer.Image;


import Valhalla.Renderer.RendererAPI;
import Valhalla.Core.Ref;
import Valhalla.Core.Assert;
import Valhalla.Core.Buffer;

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

			Assert(false, "Unknown RendererAPI");
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

			Assert(false, "Unknown RendererAPI");
			return nullptr;
		}
	}
}