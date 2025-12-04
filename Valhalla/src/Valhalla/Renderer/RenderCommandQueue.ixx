export module Valhalla.Renderer.RenderCommandQueue;

import Valhalla.Core.Types;

export namespace Valhalla
{
	export namespace Heimdall
	{
		class RenderCommandQueue
		{
		public:
			typedef void(*RenderCommandFn)(void*);

			RenderCommandQueue();
			~RenderCommandQueue();

			void* Allocate(RenderCommandFn func, uint32_t size);

			void Execute();

			uint32_t GetCount() { return m_CommandCount; }

		private:
			uint8_t* m_CommandBuffer;
			uint8_t* m_CommandBufferPtr;
			uint32_t m_CommandCount = 0;
		};
	}
}