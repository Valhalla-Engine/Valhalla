#include "vhpch.h"
#include "RendererContext.h"

#include "RendererAPI.h"

#include "Valhalla/Platform/OpenGL/OpenGLContext.h"

namespace Valhalla::Heimdal
{
	Ref<RendererContext> RendererContext::Create(void* windowHandle)
	{
		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:		return nullptr;
			case RendererAPIType::OpenGL:	return Ref<Yggdrasil::OpenGLContext>::Create(windowHandle);
		}
	}
}