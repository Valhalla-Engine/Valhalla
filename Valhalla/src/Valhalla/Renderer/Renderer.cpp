#include "vhpch.h"
#include "Renderer.h"

namespace Valhalla
{
	namespace Heimdal
	{
		RendererAPIType RendererAPI::s_CurrentRendererAPI = RendererAPIType::OpenGL;

		void RendererAPI::SetAPI(RendererAPIType api)
		{
			s_CurrentRendererAPI = api;
		}
	}
}