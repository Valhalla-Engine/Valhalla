#include "vhpch.h"
#include "RendererStats.h"

namespace Valhalla
{
	namespace Heimdall
	{
		namespace RendererUtils
		{
			static ResourceAllocationCounts s_ResourceAllocationCounts;
			ResourceAllocationCounts& GetResourceAllocationCounts()
			{
				return s_ResourceAllocationCounts;
			}
		}
	}
}