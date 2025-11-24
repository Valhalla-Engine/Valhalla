export module Valhalla.Renderer.SwapChain;

import Valhalla.Core.Ref;
import Valhalla.Renderer.RendererContext;

export namespace Valhalla
{
	export namespace Heimdall
	{
		export class SwapChain : public RefCounted
		{
		public:
			SwapChain() = default;
			virtual ~SwapChain() = default;
			virtual void Init(Ref<RendererContext>& context) = 0;
			virtual void InitSurface() = 0;
			virtual void CreateSwapchain(uint32_t* width, uint32_t* height, bool vsync) = 0;
			virtual void Destroy() = 0;

			virtual void Present() = 0;
			virtual void BeginFrame() = 0;

			virtual uint32_t GetImageCount() const = 0;
			virtual uint32_t GetCurrentBackBufferIndex() const = 0;

			virtual void OnResize(uint32_t width, uint32_t height) = 0;

			static Ref<SwapChain> Create();
		};
	}
}