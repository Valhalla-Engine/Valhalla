module Valhalla.Renderer.FrameBuffer;

import Valhalla.Renderer.RendererAPI;

namespace Valhalla
{
	namespace Heimdall
	{
		Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
		{
			Ref<FrameBuffer> result = nullptr;
			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::DirectX12:	return nullptr;
				case RendererAPIType::Vulkan:		return nullptr;
			}

			return result;
		}
	}
}