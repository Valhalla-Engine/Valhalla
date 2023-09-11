#include "vhpch.h"
#include "Renderer.h"

#include "RendererAPI.h"

#include "Valhalla/Platform/DirectX12/DirectX12Renderer.h"

namespace Valhalla
{
	namespace Heimdall
	{
		static RendererAPI* s_RendererAPI = nullptr;

		static RendererAPI* InitRendererAPI()
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::DirectX12:	return new Yggdrasil::DirectX12Renderer;
			}
			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}

		void Renderer::Init()
		{
			s_RendererAPI = InitRendererAPI();

			s_RendererAPI->Init();
		}

		void Renderer::Shutdown()
		{
			s_RendererAPI->Shutdown();
		}

		void Renderer::BeginFrame()
		{
			s_RendererAPI->BeginFrame();
		}

		void Renderer::EndFrame()
		{
			s_RendererAPI->EndFrame();
		}
	}
}