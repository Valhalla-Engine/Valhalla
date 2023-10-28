#pragma once

#include "Valhalla/Core/Ref.h"


namespace Valhalla
{

	namespace Heimdall
	{
		class RenderCommandBuffer : public RefCounted
		{
			virtual void Begin() = 0;
			virtual void End() = 0;
			virtual void Submit() = 0;

			Ref<RenderCommandBuffer> Create(uint32_t count = 0, const std::string& debugName = "");
			Ref<RenderCommandBuffer> CreateFromSwapChain(const std::string& debugName = "");
		};
	}
}