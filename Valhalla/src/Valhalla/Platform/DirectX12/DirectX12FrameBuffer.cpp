module Valhalla.Yggdrasil.DirectX12.FrameBuffer;

import Valhalla.Core.Application;
import Valhalla.Core.Assert;

import Valhalla.Renderer.Image;
import Valhalla.Renderer.Renderer;

import <format>;
import <string>;

namespace Valhalla
{
	namespace Yggdrasil
	{
		DirectX12FrameBuffer::DirectX12FrameBuffer(const Heimdall::FrameBufferSpecification& spec)
			: m_Specification(spec)
		{
			if (spec.Width == 0)
			{
				m_Width = Application::Get().GetWindow().GetWidth();
				m_Height = Application::Get().GetWindow().GetHeight();
			}
			else
			{
				m_Width = (uint32_t)(spec.Width * spec.Scale);
				m_Height = (uint32_t)(spec.Height * spec.Scale);
			}

			uint32_t attachmentIndex = 0;
			if (!m_Specification.ExisitingFrameBuffer)
			{
				for (auto& attachmentSpec : m_Specification.Attachments.Attachments)
				{
					if (m_Specification.ExistingImage)
					{
						if (Heimdall::Utils::IsDepthFormat(attachmentSpec.Format))
							m_DepthAttachmentImage = m_Specification.ExistingImage;
						else
							m_AttachmentImages.emplace_back(m_Specification.ExistingImage);
					}
					else if (m_Specification.ExistingImages.find(attachmentIndex) != m_Specification.ExistingImages.end())
					{
						if (Heimdall::Utils::IsDepthFormat(attachmentSpec.Format))
							m_DepthAttachmentImage = m_Specification.ExistingImages.at(attachmentIndex);
						else
							m_AttachmentImages.emplace_back();
					}
					else if (Heimdall::Utils::IsDepthFormat(attachmentSpec.Format))
					{
						Heimdall::ImageSpecification spec;
						spec.Format = attachmentSpec.Format;
						spec.Usage = Heimdall::ImageUsage::Attachment;
						spec.Transfer = m_Specification.Transfer;
						spec.Width = (uint32_t)(m_Width * m_Specification.Scale);
						spec.Height = (uint32_t)(m_Height * m_Specification.Scale);
						spec.DebugName = std::format("{}-DepthAttachment{}", m_Specification.DebugName.empty() ? "Unnamed FB" : m_Specification.DebugName, attachmentIndex);
						m_DepthAttachmentImage = Heimdall::Image2D::Create(spec);
					}
					else
					{
						Heimdall::ImageSpecification spec;
						spec.Format = attachmentSpec.Format;
						spec.Usage = Heimdall::ImageUsage::Attachment;
						spec.Transfer = m_Specification.Transfer;
						spec.Width = (uint32_t)(m_Width * m_Specification.Scale);
						spec.Height = (uint32_t)(m_Height * m_Specification.Scale);
						spec.DebugName = std::format("{}-DepthAttachment{}", m_Specification.DebugName.empty() ? "Unnamed FB" : m_Specification.DebugName, attachmentIndex);
						m_AttachmentImages.emplace_back(Heimdall::Image2D::Create(spec));
					}
					attachmentIndex++;
				}
			}

			Assert(spec.Attachments.Attachments.size());
			Resize(m_Width, m_Height, true);
		}

		DirectX12FrameBuffer::~DirectX12FrameBuffer()
		{
		}

		void DirectX12FrameBuffer::Resize(uint32_t width, uint32_t height, bool forceRecreate)
		{
			if (!forceRecreate && (m_Width == width && m_Height == height))
				return;

			Ref<DirectX12FrameBuffer> instance = this;
			Heimdall::Renderer::Submit([instance, width, height]() mutable 
			{
				instance->m_Width = (uint32_t)(width * instance->m_Specification.Scale);
				instance->m_Height = (uint32_t)(height * instance->m_Specification.Scale);

				if (!instance->m_Specification.SwapChainTarget)
				{
					instance->RT_Invalidate();
				}
				else
				{
					instance->m_ClearValues.clear();
					instance->m_ClearValues.emplace_back().Color[0] = 0.0f;
					instance->m_ClearValues.emplace_back().Color[1] = 0.0f;
					instance->m_ClearValues.emplace_back().Color[2] = 0.0f;
					instance->m_ClearValues.emplace_back().Color[3] = 1.0f;
				}
			});

			for (auto& callback : m_ResizeCallbacks)
				callback(this);
		}

		void DirectX12FrameBuffer::AddResizeCallback(const std::function<void(Ref<FrameBuffer>)>& func)
		{
			m_ResizeCallbacks.push_back(func);
		}
	}
}