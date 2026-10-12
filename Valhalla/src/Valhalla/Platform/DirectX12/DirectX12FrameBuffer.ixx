module;
#include "D12Include.hpp"

export module Valhalla.Yggdrasil.DirectX12.FrameBuffer;

import Valhalla.Renderer.FrameBuffer;

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

			virtual Ref<Heimdall::Image2D> GetImage(uint32_t attachmentIndex = 0) const override { return m_AttachmentImages[attachmentIndex]; }
			virtual Ref<Heimdall::Image2D> GetDepthImage() const override { return m_DepthAttachmentImage; }
			virtual size_t GetColorAttachmentCount() const override { return m_Specification.SwapChainTarget ? 1 : m_AttachmentImages.size(); }
			virtual bool HasDepthAttachment() const { return (bool)m_DepthAttachmentImage; }
			
			void Invalidate();
			void RT_Invalidate();

		private:
			Heimdall::FrameBufferSpecification m_Specification;
			uint32_t m_Width = 0;
			uint32_t m_Height = 0;

			std::vector<D3D12_CLEAR_VALUE> m_ClearValues;

			Microsoft::WRL::ComPtr<ID3D12Resource> m_DepthStencilBuffer;
			Microsoft::WRL::ComPtr<ID3D12Resource> m_FrameBuffer;
			std::vector<Ref<Heimdall::Image2D>> m_AttachmentImages;
			Ref<Heimdall::Image2D> m_DepthAttachmentImage;

			std::vector<std::function<void(Ref<Heimdall::FrameBuffer>)>> m_ResizeCallbacks;
		};
	}
}