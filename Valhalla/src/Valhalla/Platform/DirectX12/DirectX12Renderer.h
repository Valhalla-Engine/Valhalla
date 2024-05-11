#pragma once
#include "VD3D12.h"
#include "Valhalla/Renderer/RendererAPI.h"

#include "DirectX12Context.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		class DirectX12Renderer : public Heimdall::RendererAPI
		{
		public:

			~DirectX12Renderer() = default;
			virtual void Init() override;
			virtual void Shutdown() override;

			virtual void BeginFrame() override;
			virtual void EndFrame() override;

			virtual void BeginRenderPass(Ref<Heimdall::RenderCommandBuffer> renderCommandBuffer, Ref<Heimdall::RenderPass> renderPass, bool explicitClear = false) override;
			virtual void EndRenderPass(Ref<Heimdall::RenderCommandBuffer> renderCommandBuffer) override;

			virtual void SubmitFillScreenQuad(Ref<Heimdall::RenderCommandBuffer> renderCommandBuffer, Ref<Heimdall::Pipeline> pipeline, Ref<Heimdall::Shader> shader) override;
			virtual void SubmitFullscreenQuadWithOverrides(Ref<Heimdall::RenderCommandBuffer> renderCommandBuffer, Ref<Heimdall::Pipeline> pipeline, Ref<Heimdall::Shader> shader, Buffer vertexShaderOverrides, Buffer fragmentShaderOverrides) override;

			virtual std::pair<Ref<Heimdall::TextureCube>, Ref<Heimdall::TextureCube>> CreateEnvironmentMap(const std::string& filepath) override;
			virtual Ref<Heimdall::TextureCube> CreatePreethamSky(float turbidity, float azimuth, float inclination) override;

			virtual void RenderGeometry(Ref<Heimdall::RenderCommandBuffer> renderCommandBuffer, Ref<Heimdall::Pipeline> pipeline, Ref<Heimdall::Shader> shader, Ref<Heimdall::VertexBuffer> vertexBuffer, Ref<Heimdall::IndexBuffer> indexBuffer, const DirectX::XMFLOAT4X4& transform, uint32_t indexCount = 0) override;

			void WaitForFrame();


		private:
			Microsoft::WRL::ComPtr<ID3D12RootSignature> m_RootSignature;

			Ref<DirectX12Context> m_Context;

			CD3DX12_VIEWPORT m_viewport;
			CD3DX12_RECT m_ScissorRect;

			inline static DirectX12Renderer* instance;
		};
	}
}