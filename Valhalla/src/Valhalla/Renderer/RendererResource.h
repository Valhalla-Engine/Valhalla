#pragma once

#include "Valhalla/Asset/Asset.h"

namespace Valhalla
{
	namespace Heimdall
	{
		using ResourceDescriptorInfo = void*;

		class RendererResource : public Mimir::Asset
		{
		public:
			virtual ResourceDescriptorInfo GetDescriptorInfo() const = 0;
		};
	}
}