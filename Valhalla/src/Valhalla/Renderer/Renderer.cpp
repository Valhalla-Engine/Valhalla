#include "vhpch.h"
#include "Renderer.h"

#include "RendererAPI.h"

#include "Valhalla/Platform/DirectX12/DirectX12Renderer.h"

namespace Valhalla
{
	namespace Heimdall
	{
		static RendererAPI* s_RendererAPI = nullptr;

		struct ShaderDependencies
		{
			std::vector<Ref<Pipeline>> Pipelines;

		};

		struct RendererData
		{
			Ref<ShaderLibrary> m_ShaderLibrary;
		};

		static std::unordered_map<size_t, ShaderDependencies> s_ShaderDependencies;

		static RendererConfig s_Config;

		static RendererData* s_Data = nullptr;
		constexpr static uint32_t s_RenderCommandQueueCount = 2;
		static RenderCommandQueue* s_CommandQueue;
		static std::atomic<uint32_t> s_RenderCommandQueueSubmissionIndex = 0;
		static RenderCommandQueue s_ResourceFreeQueue[3];

		void Renderer::RegisterShaderDependency(Ref<Shader> shader, Ref<Pipeline> pipeline)
		{
			s_ShaderDependencies[shader->GetHash()].Pipelines.push_back(pipeline);
		}

		Ref<ShaderLibrary> Renderer::GetShaderLibrary()
		{
			return s_Data->m_ShaderLibrary;
		}

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
			s_Data = new RendererData();

			s_CommandQueue = new RenderCommandQueue();

			//s_Config.FramesInFlight = std::min<uint32_t>(s_Config.FramesInFlight, Application::Get().GetWindow().GetSwapChain()->GetImageCount());

			s_RendererAPI = InitRendererAPI();

			s_Data->m_ShaderLibrary = Ref<ShaderLibrary>::Create();

			//Renderer::GetShaderLibrary()->Load("Resources/Shaders/Shader.hlsl");

			s_RendererAPI->Init();
		}

		void Renderer::Shutdown()
		{
			s_RendererAPI->Shutdown();

			delete s_Data;

			for (size_t i = 0; i < s_Config.FramesInFlight; i++)
			{
				auto& queue = Renderer::GetRenderResourceReleaseQueue(i);
				queue.Execute();
			}

			delete s_CommandQueue;
		}

		void Renderer::WaitAndRender()
		{
			s_CommandQueue->Execute();
		}

		uint32_t Renderer::GetRenderQueueIndex()
		{
			return (s_RenderCommandQueueSubmissionIndex + 1) % s_RenderCommandQueueCount;
		}

		void Renderer::BeginFrame()
		{
			s_RendererAPI->BeginFrame();
		}

		void Renderer::EndFrame()
		{
			s_RendererAPI->EndFrame();
		}

		uint32_t Renderer::RT_GetCurrentFrameIndex()
		{
			return Application::Get().GetWindow().GetSwapChain()->GetCurrentBackBufferIndex();
		}

		RendererConfig& Renderer::GetConfig()
		{
			return s_Config;
		}

		void Renderer::SetConfig(const RendererConfig& config)
		{
			s_Config = config;
		}

		RenderCommandQueue& Renderer::GetRenderResourceReleaseQueue(uint32_t index)
		{
			return s_ResourceFreeQueue[index];
		}

		RenderCommandQueue& Renderer::GetRenderCommandQueue()
		{
			return *s_CommandQueue;
		}
	}
}