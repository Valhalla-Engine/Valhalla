export module Valhalla.Renderer.RendererStats;

import Valhalla.Core.Types;

export namespace Valhalla
{
	export namespace Heimdall
	{
		export namespace RendererUtils
		{
			struct ResourceAllocationCounts
			{
				uint32_t Samplers = 0;
			};

			ResourceAllocationCounts& GetResourceAllocationCounts();
		}
	}
}