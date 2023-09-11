#pragma once

#include "RendererContext.h"

#include "Valhalla/Core/Application.h"

namespace Valhalla
{
	namespace Heimdall
	{
		class Renderer
		{
		public:
			static Ref<RendererContext> GetContext()
			{
				return Application::Get().GetWindow().GetRendererContext();
			}

			static void Init();
			static void Shutdown();

			static void BeginFrame();
			static void EndFrame();
		};
	}
}