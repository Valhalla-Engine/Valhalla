#pragma once

#include "Valhalla/Renderer/FrameBuffer.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		class OpenGLFrameBuffer : public Heimdal::FrameBuffer
		{
		public:
			OpenGLFrameBuffer(const Heimdal::FrameBufferSpecification& spec);
			virtual ~OpenGLFrameBuffer();

			virtual void Resize(uint32_t width, uint32_t height, bool forceRecreate = false) override;

			virtual void Bind() const override;
			virtual void Unbind() const override;

			virtual void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const override;

			virtual uint32_t GetWidth() const override { return m_Width; }
			virtual uint32_t GetHeight() const override { return m_Height; }

			virtual Heimdal::RendererID GetRendererID() const override { return m_RendererID; }
			virtual Heimdal::RendererID GetColorAttachmentRendererID(int32_t index = 0) const override { return m_ColorAttachments[index]; }
			virtual Heimdal::RendererID GetDepthAttachmentRendererID() const override { return m_DepthAttachment; }

			virtual const Heimdal::FrameBufferSpecification& GetSpecification() const override { return m_Specification; }

		private:
			Heimdal::FrameBufferSpecification m_Specification;
			Heimdal::RendererID m_RendererID = 0;

			std::vector<Heimdal::RendererID> m_ColorAttachments;
			Heimdal::RendererID m_DepthAttachment = 0;

			std::vector<Heimdal::FrameBufferTextureFormat> m_ColorAttachmentFormats;
			Heimdal::FrameBufferTextureFormat m_DepthAttachmentFormat = Heimdal::FrameBufferTextureFormat::None;

			uint32_t m_Width = 0, m_Height = 0;
		};
	}
}