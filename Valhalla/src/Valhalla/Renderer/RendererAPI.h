 #pragma once

#include "RendererCapabilities.h"
#include "RenderCommandBuffer.h"
#include "StorageBufferSet.h"
#include "UniformBufferSet.h"
#include "RenderPass.h"
#include "IndexBuffer.h"

namespace Valhalla
{
	namespace Heimdall
	{
		enum class RendererAPIType
		{
			None, DirectX11, DirectX12, Vulkan
		};

		class RendererAPI
		{
		public:
			virtual void Init() = 0;
			virtual void Shutdown() = 0;

			virtual void BeginFrame() = 0;
			virtual void EndFrame() = 0;

			static RendererAPIType Current() { return s_CurrentRendererAPI; }
			static void SetAPI(RendererAPIType api);
		private:
			inline static RendererAPIType s_CurrentRendererAPI = RendererAPIType::DirectX12;
		};
	}
}