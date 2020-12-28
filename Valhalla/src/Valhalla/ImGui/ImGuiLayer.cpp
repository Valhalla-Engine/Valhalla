#include "vhpch.h"
#include "ImGuiLayer.h"

#include "Valhalla/Renderer/Renderer.h"
namespace Valhalla
{
	ImGuiLayer* ImGuiLayer::Create()
	{
		switch (Heimdal::RendererAPI::Current())
		{
			case Heimdal::RendererAPIType::None:		return nullptr;
			case Heimdal::RendererAPIType::OpenGL:		VH_CORE_ASSERT(false, "Dumb ass, you forgot to change this line of code"); return nullptr;
			case Heimdal::RendererAPIType::DirectX11:	VH_CORE_ASSERT(false, "DirectX 11 is currently not supported"); return nullptr;
			case Heimdal::RendererAPIType::DirectX12:	VH_CORE_ASSERT(false, "DirectX 12 is currently not supported"); return nullptr;
			case Heimdal::RendererAPIType::Vulkan:		VH_CORE_ASSERT(false, "Vulkan is currently not supported");  return nullptr;
			case Heimdal::RendererAPIType::Metal:		VH_CORE_ASSERT(false, "Metal is currently not supported"); return nullptr;
		}
	}
}