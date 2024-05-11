#include "vhpch.h"
#include "DirectX12Renderer.h"
#include "Valhalla/Core/Application.h"

#include "DirectX12Context.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		void DirectX12Renderer::Init()
		{
			
		}

		void DirectX12Renderer::Shutdown()
		{

		}

		void DirectX12Renderer::BeginFrame()
		{
			
		}

		void DirectX12Renderer::EndFrame()
		{
			
		}

		void DirectX12Renderer::WaitForFrame()
		{

		}

		void DirectX12Renderer::BeginRenderPass(Ref<Heimdall::RenderCommandBuffer> buffer, Ref<Heimdall::RenderPass> pass, bool explicitClear)
		{


		}

		void DirectX12Renderer::EndRenderPass(Ref<Heimdall::RenderCommandBuffer> commandBuffer)
		{

		}

		void DirectX12Renderer::SubmitFillScreenQuad(Ref<Heimdall::RenderCommandBuffer> renderCommandBuffer, Ref<Heimdall::Pipeline> pipeline, Ref<Heimdall::Shader> shader)
		{

		}

		void DirectX12Renderer::SubmitFullscreenQuadWithOverrides(Ref<Heimdall::RenderCommandBuffer> renderCommandBuffer, Ref<Heimdall::Pipeline> pipeline, Ref<Heimdall::Shader> shader, Buffer vertexShaderOverrides, Buffer fragmentShaderOverrides)
		{

		}

		std::pair<Ref<Heimdall::TextureCube>, Ref<Heimdall::TextureCube>> DirectX12Renderer::CreateEnvironmentMap(const std::string& filepath)
		{
			return {};
		}

		Ref<Heimdall::TextureCube> DirectX12Renderer::CreatePreethamSky(float turbidity, float azimuth, float inclination)
		{
			return nullptr;
		}

		void DirectX12Renderer::RenderGeometry(Ref<Heimdall::RenderCommandBuffer> renderCommandBuffer, Ref<Heimdall::Pipeline> pipeline, Ref<Heimdall::Shader> shader, Ref<Heimdall::VertexBuffer> vertexBuffer, Ref<Heimdall::IndexBuffer> indexBuffer, const DirectX::XMFLOAT4X4& transform, uint32_t indexCount)
		{

		}
	}
}