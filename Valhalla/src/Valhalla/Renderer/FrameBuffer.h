#pragma once

#include "Valhalla/Renderer/Image.h"

#include "Valhalla/Core/Ref.h"

namespace Valhalla
{
	namespace Heimdall
	{
		class FrameBuffer;

		enum class FrameBufferBlendMode
		{
			None = 0,
			OneZero,
			SrcAlphaOneMinusSrcAlpha,
			Additive,
			Zero_SrcColor
		};

		struct FrameBufferTextureSpecification
		{
			FrameBufferTextureSpecification() = default;
			FrameBufferTextureSpecification(ImageFormat format) : Format(format) {}


			ImageFormat Format;
			bool Blend = true;
			FrameBufferBlendMode BlendMode = FrameBufferBlendMode::SrcAlphaOneMinusSrcAlpha;
		};

		struct FrameBufferAttachmentsSpecification
		{
			FrameBufferAttachmentsSpecification() = default;
			FrameBufferAttachmentsSpecification(std::initializer_list<FrameBufferTextureSpecification>& attachments) 
				: Attachments(attachments) {}

			std::vector<FrameBufferTextureSpecification> Attachments;
		};

		struct FrameBufferSpecification
		{
			float_t Scale = 1.0f;
			uint32_t Width = 0;
			uint32_t Height = 0;
			float_t ClearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
			float_t DepthClearValue = 0.0f;
			bool ClearColorOnLoad = true;
			bool ClearDepthOnLoad = true;

			FrameBufferAttachmentsSpecification Attachments;
			uint32_t Samples = 1;

			bool NoResize = false;

			bool Blend = true;

			FrameBufferBlendMode BlendMode = FrameBufferBlendMode::None;

			bool SwapChainTarget = false;

			bool Transfer = false;

			Ref<Image2D> ExistingImage;
			std::vector<uint32_t> ExistingImageLayer;

			std::map<uint32_t, Ref<Image2D>> ExistingImages;

			Ref<FrameBuffer> ExisitingFrameBuffer;

			std::string DebugName;
		};

		class FrameBuffer : public RefCounted
		{
		public:
			virtual ~FrameBuffer() {}
			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			virtual void Resize(uint32_t width, uint32_t height, bool forceRecreate = false) = 0;
			virtual void AddResizeCallback(const std::function<void(Ref<FrameBuffer>)>& func) = 0;

			virtual void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const = 0;

			virtual uint32_t GetWidth() const = 0;
			virtual uint32_t GetHeight() const = 0;

			virtual Ref<Image2D> GetImage(uint32_t attachmentIndex = 0) const = 0;
			virtual size_t GetColorAttachmentCount() const = 0;
			virtual bool HasDepthAttachment() const = 0;
			virtual Ref<Image2D> GetDepthImage() const = 0;

			virtual const FrameBufferSpecification& GetSpecifications() const = 0;

			static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
		};
	}
}