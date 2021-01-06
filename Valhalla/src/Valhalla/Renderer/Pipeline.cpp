#include "vhpch.h"
#include "Pipeline.h"

#include "Renderer.h"

#include "Valhalla/Platform/OpenGL/OpenGLPipeline.h"

namespace Valhalla::Heimdal
{
	Ref<Pipeline> Pipeline::Create(const PipelineSpecification& spec)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:		return nullptr;
			case RendererAPIType::OpenGL:	return Ref<Yggdrasil::OpenGLPipeline>::Create(spec);
		}
		VH_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}