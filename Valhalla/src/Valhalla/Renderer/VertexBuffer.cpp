module Valhalla.Renderer.VertexBuffer;

import Valhalla.Core.Assert;
import Valhalla.Core.Ref;

import Valhalla.Renderer.RendererAPI;

namespace Valhalla
{
	namespace Heimdall
	{
		Ref<VertexBuffer> VertexBuffer::Create(void* data, uint32_t size, VertexBufferUsage usage)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::DirectX11:	return nullptr;
				case RendererAPIType::DirectX12:	return nullptr;
				case RendererAPIType::Vulkan:		return nullptr;
			}
			Assert(false, "Unknown RendererAPI");
			return nullptr;
		}

		Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, uint32_t stride, VertexBufferUsage usage)
		{
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::DirectX11:	return nullptr;
				case RendererAPIType::DirectX12:	return nullptr;
				case RendererAPIType::Vulkan:		return nullptr;
			}
			Assert(false, "Unknown RendererAPI");
			return nullptr;
		}
	}
}