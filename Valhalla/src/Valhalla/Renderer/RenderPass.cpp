#include "vhpch.h"
#include "RenderPass.h"

#include "Renderer.h"

#include "Valhalla/Platform/OpenGL/OpenGLRenderPass.h"

namespace Valhalla::Heimdal
{
	Ref<RenderPass> RenderPass::Create(const RenderPassSpecification& spec)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:		return nullptr;
			case RendererAPIType::OpenGL:	return Ref<Yggdrasil::OpenGLRenderPass>::Create(spec);
		}

		VH_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}