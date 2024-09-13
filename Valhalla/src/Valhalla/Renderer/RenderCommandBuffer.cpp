#include "vhpch.h"
#include "RenderCommandBuffer.h"

#include "RendererAPI.h"

#include "Valhalla/Platform/DirectX12/DirectX12RenderCommandBuffer.h"

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
				case RendererAPIType::DirectX12:		return Ref<Yggdrasil::DirectX12RenderCommandBuffer>::Create(count, debugName);
				case RendererAPIType::Vulkan:			return nullptr;
			}
			VH_CORE_ASSERT(false, "Unknown RendererAPI");
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
			VH_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
	}
}