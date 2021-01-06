#pragma once

#include "RenderCommandQueue.h"
#include "RenderPass.h"

namespace Valhalla
{
	namespace Heimdal
	{
		class VALHALLA_API Renderer
		{
		public:

			static void Init();

			template<typename FuncT>
			static void Submit(FuncT&& func)
			{
				auto renderCmd = [](void* ptr)
				{
					auto pFunc = (FuncT*)ptr;
					(*pFunc)();

					pFunc->~FuncT();
				};

				auto storageBuffer = GetRenderCommandQueue().Allocate(renderCmd, sizeof(func));
				new (storageBuffer) FuncT(std::forward<FuncT>(func));
			}

		private:
			static RenderCommandQueue& GetRenderCommandQueue();
		};
	}
}