#pragma once
#include "Valhalla/Core/Ref.h"

namespace Valhalla
{
	namespace Heimdall
	{
		class SwapChain : public RefCounted
		{
		public:
			SwapChain() = default;
			virtual ~SwapChain() = default;
			virtual void Init() = 0;

			virtual void Present() = 0;
			virtual void BeginFrame() = 0;

			virtual void OnResize(uint32_t width, uint32_t height) = 0;

			static Ref<SwapChain> Create();
		};
	}
}