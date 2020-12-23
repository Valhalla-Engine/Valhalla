#include "vhpch.h"
#include "Texture.h"

#include "RendererAPI.h"

namespace Valhalla
{
	namespace Heimdal
	{
		Ref<Texture2D> Texture2D::Create(TextureFormat format, uint32_t width, uint32_t height, TextureWrap wrap)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::OpenGL:		return nullptr;
				case RendererAPIType::DirectX11:	VH_CORE_ERROR("DirectX11 not currently supported"); return nullptr;
				case RendererAPIType::DirectX12:	VH_CORE_ERROR("DirectX12 not currently supported"); return nullptr;
				case RendererAPIType::Vulkan:		VH_CORE_ERROR("Vulkan not currently supported"); return nullptr;
				case RendererAPIType::Metal:		VH_CORE_ERROR("Metal not currently supported");  return nullptr;
			}
			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}

		Ref<Texture2D> Texture2D::Create(const std::string& path, bool srgb)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::OpenGL:		return nullptr;
				case RendererAPIType::DirectX11:	VH_CORE_ERROR("DirectX11 not currently supported"); return nullptr;
				case RendererAPIType::DirectX12:	VH_CORE_ERROR("DirectX12 not currently supported"); return nullptr;
				case RendererAPIType::Vulkan:		VH_CORE_ERROR("Vulkan not currently supported"); return nullptr;
				case RendererAPIType::Metal:		VH_CORE_ERROR("Metal not currently supported");  return nullptr;
			}
			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}

		Ref<TextureCube> TextureCube::Create(TextureFormat format, uint32_t width, uint32_t height)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::OpenGL:		return nullptr;
				case RendererAPIType::DirectX11:	VH_CORE_ERROR("DirectX11 not currently supported"); return nullptr;
				case RendererAPIType::DirectX12:	VH_CORE_ERROR("DirectX12 not currently supported"); return nullptr;
				case RendererAPIType::Vulkan:		VH_CORE_ERROR("Vulkan not currently supported"); return nullptr;
				case RendererAPIType::Metal:		VH_CORE_ERROR("Metal not currently supported");  return nullptr;
			}
			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}

		Ref<TextureCube> TextureCube::Create(const std::string& path)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::OpenGL:		return nullptr;
				case RendererAPIType::DirectX11:	VH_CORE_ERROR("DirectX11 not currently supported"); return nullptr;
				case RendererAPIType::DirectX12:	VH_CORE_ERROR("DirectX12 not currently supported"); return nullptr;
				case RendererAPIType::Vulkan:		VH_CORE_ERROR("Vulkan not currently supported"); return nullptr;
				case RendererAPIType::Metal:		VH_CORE_ERROR("Metal not currently supported");  return nullptr;
			}
			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}

		uint32_t Texture::GetBPP(TextureFormat format)
		{
			switch (format)
			{
				case TextureFormat::RGB:	return 3;
				case TextureFormat::RGBA:	return 4;
			}
			return 0;
		}

		uint32_t Texture::CalculateMipMapCount(uint32_t width, uint32_t height)
		{
			uint32_t levels = 1;

			while ((width | height) >> levels)
				levels++;

			return levels;
		}
	}
}