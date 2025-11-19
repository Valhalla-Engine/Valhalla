export module Valhalla.Renderer.IndexBuffer;

import Valhalla.Core.Ref;
import Valhalla.Core.Types;

export namespace Valhalla
{
	export namespace Heimdall
	{
		class IndexBuffer : public RefCounted
		{
		public:

			virtual ~IndexBuffer() = default;

			virtual void SetData(void* buffer, uint16_t size, uint16_t offset) = 0;

			virtual uint16_t GetCount() const = 0;

			virtual uint16_t GetSize() const = 0;

			static Ref<IndexBuffer> Create(uint16_t size);
			static Ref<IndexBuffer> Create(void* data, uint16_t size = 0);
		};
	}
}