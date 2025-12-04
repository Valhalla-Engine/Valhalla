module Valhalla.Renderer.Texture;

import Valhalla.Renderer.RendererAPI;
import Valhalla.Core.Assert;
import Valhalla.Core.Ref;

namespace Valhalla
{
	namespace Heimdall
	{
		Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification)
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

		Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification, const std::filesystem::path& filepath)
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

		Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification, Buffer imageData)
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

		Ref<TextureCube> TextureCube::Create(const TextureSpecification& specification, Buffer imageData)
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