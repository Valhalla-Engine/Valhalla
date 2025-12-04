export module Valhalla.Renderer.RendererResource;

import Valhalla.Asset;

export namespace Valhalla
{
	export namespace Heimdall
	{
		using ResourceDescriptorInfo = void*;

		class RendererResource : public Mimir::Asset
		{
		public:
			virtual ResourceDescriptorInfo GetDescriptorInfo() const = 0;
		};
	}
}