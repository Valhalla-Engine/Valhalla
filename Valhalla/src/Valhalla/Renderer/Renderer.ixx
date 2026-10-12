module;
#include <new>
#include <utility>
#include <type_traits>
#include <cstdint>

export module Valhalla.Renderer.Renderer;

import Valhalla.Renderer.RendererContext;
import Valhalla.Renderer.RendererConfig;
import Valhalla.Renderer.RenderCommandQueue;
import Valhalla.Renderer.IndexBuffer;
import Valhalla.Renderer.VertexBuffer;

import Valhalla.Core.Types;

import Valhalla.Renderer.Pipeline;
import Valhalla.Renderer.Shader;

import Valhalla.Core.Application;

import Valhalla.Core.Ref;



export namespace Valhalla
{
	export namespace Heimdall
	{
		class Renderer
		{
		public:
			static Ref<RendererContext> GetContext()
			{
				return Application::Get().GetWindow().GetRendererContext();
			}

			static Ref<ShaderLibrary> GetShaderLibrary();

			static void Init();
			static void Shutdown();

			template<typename FuncT>
			static void Submit(FuncT&& func)
			{
				using StoredFuncT = std::decay_t<FuncT>;

				auto renderCmd = [](void* ptr)
					{
						auto* pFunc = static_cast<StoredFuncT*>(ptr);

						(*pFunc)();
						pFunc->~StoredFuncT();
					};

				auto storageBuffer =
					GetRenderCommandQueue().Allocate(
						renderCmd,
						sizeof(StoredFuncT)
					);

				new (storageBuffer)
					StoredFuncT(std::forward<FuncT>(func));
			}

			template<typename FuncT>
			static void SubmitResourceFree(FuncT&& func)
			{
				auto renderCmd = [](void* ptr) 
				{
					auto pFunc = (FuncT*)ptr;
					(*pFunc)();

					pFunc->~FuncT();
				};

				Submit([renderCmd, func]() 
				{
					const uint32_t index = Renderer::RT_GetCurrentFrameIndex();
					auto storageBuffer = GetRenderResourceReleaseQueue(index).Allocate(renderCmd, sizeof(func));
					new (storageBuffer) FuncT(std::forward<FuncT>((FuncT&&)func));
				});
			}

			static void WaitAndRender();
			static void SwapQueues();

			static void RenderMesh(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer);

			static uint32_t GetRenderQueueIndex();

			static void BeginFrame();
			static void EndFrame();

			static void RegisterShaderDependency(Ref<Shader> shader, Ref<Pipeline> pipeline);

			static uint32_t RT_GetCurrentFrameIndex();

			static RendererConfig& GetConfig();
			static void SetConfig(const RendererConfig& config);


			static RenderCommandQueue& GetRenderResourceReleaseQueue(uint32_t index);

		private:
			static RenderCommandQueue& GetRenderCommandQueue();
		};
	}
}