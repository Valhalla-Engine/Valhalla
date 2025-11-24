export module Valhalla.Renderer.RendererConfig;

import Valhalla.Core.Types;

export namespace Valhalla
{
	export namespace Heimdall
	{
		struct RendererConfig
		{
			uint32_t FramesInFlight = 3;
		};
	}
}