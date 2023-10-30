#pragma once

#include "VD3D12.h"

#include "Valhalla/Renderer/FrameBuffer.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		class DirectX12FrameBuffer : public Heimdall::FrameBuffer
		{
		public:
			DirectX12FrameBuffer(const Heimdall::FrameBufferSpecification& spec);
			virtual ~DirectX12FrameBuffer();

			virtual void Resize(uint32_t width, uint32_t height, bool forceRecreate = false) override;
			virtual void AddResizeCallback(const std::function<void(Ref<FrameBuffer>)>& func) override;

			virtual void Bind() const override {}
			virtual void Unbind() const override {}

			virtual void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const override {}

			virtual uint32_t GetWidth() const override { return m_Width; }
			virtual uint32_t GetHeight() const override { return m_Height; }


		private:
			Heimdall::FrameBufferSpecification m_Specification;
			uint32_t m_Width = 0;
			uint32_t m_Height = 0;

			std::vector<D3D12_CLEAR_VALUE> m_ClearValues;

			/*Add DirectX Structures*/
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RTVHeap;
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DSVHeap;

			std::vector<std::function<void(Ref<Heimdall::FrameBuffer>)>> m_ResizeCallbacks;
		};
	}
}