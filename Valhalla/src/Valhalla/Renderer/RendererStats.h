#pragma once

namespace Valhalla
{
	namespace Heimdall
	{
		namespace RendererUtils
		{
			struct ResourceAllocationCounts
			{
				uint32_t Samplers = 0;
			};

			ResourceAllocationCounts& GetResourceAllocationCounts();
		}
	}
}