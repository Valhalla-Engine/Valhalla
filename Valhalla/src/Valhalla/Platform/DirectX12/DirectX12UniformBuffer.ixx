#include "D12Include.hpp"

export module Valhalla.Yggdrasil.DirectX12.UniformBuffer;

import Valhalla.Renderer.UniformBuffer;

export namespace Valhalla
{
	export namespace Yggdrasil
	{
		class DirectX12UniformBuffer : public Heimdall::UniformBuffer
		{
		public:
			DirectX12UniformBuffer(uint32_t size, uint32_t binding);

			virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
			virtual void RT_SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

			virtual uint32_t GetBinding() const override;
		private:
			Microsoft::WRL::ComPtr<ID3D12Resource> m_UniformBuffer;
		};
	}
}