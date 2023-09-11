#pragma once

#include "Valhalla/Core/Ref.h"

namespace Valhalla
{
	namespace Heimdall
	{
		class RendererContext : public RefCounted
		{
		public:
			RendererContext() = default;
			virtual ~RendererContext() = default;

			virtual void Init() = 0;

			static Ref<RendererContext> Create();
		};
	}
}