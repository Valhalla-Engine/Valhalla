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

			virtual void BeginRenderPass(Ref<RenderCommandBuffer> renderCommandBuffer, Ref<RenderPass> renderPass, bool explicitClear = false) = 0;
			virtual void EndRenderPass(Ref<RenderCommandBuffer> renderCommandBuffer) = 0;

			virtual void SubmitFillScreenQuad(Ref<RenderCommandBuffer> renderCommandBuffer, Ref<Pipeline> pipeline, Ref<Shader> shader) = 0;
			virtual void SubmitFullscreenQuadWithOverrides(Ref<RenderCommandBuffer> renderCommandBuffer, Ref<Pipeline> pipeline, Ref<Shader> shader, Buffer vertexShaderOverrides, Buffer fragmentShaderOverrides) = 0;

			virtual std::pair<Ref<TextureCube>, Ref<TextureCube>> CreateEnvironmentMap(const std::string& filepath) = 0;
			virtual Ref<TextureCube> CreatePreethamSky(float turbidity, float azimuth, float inclination) = 0;


			virtual void RenderGeometry(Ref<RenderCommandBuffer> renderCommandBuffer, Ref<Pipeline> pipeline, Ref<Shader> shader, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, const DirectX::XMFLOAT4X4& transform, uint32_t indexCount = 0) = 0;

			static RendererAPIType Current() { return s_CurrentRendererAPI; }
			static void SetAPI(RendererAPIType api);
		private:
			inline static RendererAPIType s_CurrentRendererAPI = RendererAPIType::DirectX12;
		};
	}
}