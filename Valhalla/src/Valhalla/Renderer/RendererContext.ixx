export module Valhalla.Renderer.RendererContext;

import Valhalla.Core.Ref;

export namespace Valhalla
{
	export namespace Heimdall
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