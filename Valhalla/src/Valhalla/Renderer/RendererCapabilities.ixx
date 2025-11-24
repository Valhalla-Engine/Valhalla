export module Valhalla.Renderer.RendererCapabilities;

import <string>;

export namespace Valhalla
{
	export namespace Heimdall
	{
		struct RendererCapabilities
		{
			std::string Vendor;
			std::string Device;
			std::string Version;

			int MaxSamples = 0;
			float MaxAnisotropy = 0.0f;
			int MaxTextureUnits = 0;
		};
	}
}