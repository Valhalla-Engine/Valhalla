#include "vhpch.h"
#include "Renderer.h"

#include "RendererAPI.h"

namespace Valhalla
{
	namespace Heimdal
	{
		RendererAPIType RendererAPI::s_CurrentRendererAPI = RendererAPIType::OpenGL;


		struct RendererData
		{
			RenderCommandQueue m_CommandQueue;
		};

		static RendererData s_Data;

		void Renderer::Init()
		{
			
		}



		RenderCommandQueue& Renderer::GetRenderCommandQueue()
		{
			return s_Data.m_CommandQueue;
		}
	}
}