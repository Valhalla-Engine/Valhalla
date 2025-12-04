module Valhalla.Renderer.RenderCommandBuffer;

import Valhalla.Renderer.RendererAPI;

import Valhalla.Core.Ref;
import Valhalla.Core.Assert;

namespace Valhalla
{
	namespace Heimdall
	{
		Ref<RenderCommandBuffer> RenderCommandBuffer::Create(uint32_t count, const std::string& debugName)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:				return nullptr;
				case RendererAPIType::DirectX11:		return nullptr;
				case RendererAPIType::DirectX12:		return nullptr;
				case RendererAPIType::Vulkan:			return nullptr;
			}
			Assert(false, "Unknown RendererAPI");
			return nullptr;
		}

		Ref<RenderCommandBuffer> RenderCommandBuffer::CreateFromSwapChain(const std::string& debugName)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:				return nullptr;
				case RendererAPIType::DirectX11:		return nullptr;
				case RendererAPIType::DirectX12:		return nullptr;
				case RendererAPIType::Vulkan:			return nullptr;
			}
			Assert(false, "Unknown RendererAPI");
			return nullptr;
		}
	}
}