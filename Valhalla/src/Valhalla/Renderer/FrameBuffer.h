#pragma once

#include <glm/glm.hpp>

#include "RendererAPI.h"

namespace Valhalla
{
	namespace Heimdal
	{
		enum class FrameBufferTextureFormat
		{
			None = 0,

			// Color
			RGBA8 = 1,
			RGBA16F = 2,
			RGBA32F = 3,
			RG32F = 4,

			// Depth/stencil
			DEPTH32F = 5,
			DEPTH24STENCIL8 = 6,

			// Defaults
			Depth = DEPTH24STENCIL8
		};

		struct FrameBufferTextureSpecification
		{
			FrameBufferTextureFormat TextureFormat;

			FrameBufferTextureSpecification() = default;
			FrameBufferTextureSpecification(FrameBufferTextureFormat format) : TextureFormat(format) {}
		};

		struct FrameBufferAttachmentSpecification
		{
			std::vector<FrameBufferTextureSpecification> Attachments;

			FrameBufferAttachmentSpecification() = default;
			FrameBufferAttachmentSpecification(const std::initializer_list<FrameBufferTextureSpecification>& attachments)
				: Attachments(attachments) {}
		};

		struct FrameBufferSpecification
		{
			uint32_t Width = 1280;
			uint32_t Height = 720;
			glm::vec4 ClearColor;
			FrameBufferAttachmentSpecification Attachments;
			uint32_t Samples = 1;

			bool NoResize = false;

			bool SwapChainTarget = false;
		};

		class VALHALLA_API FrameBuffer : public RefCounted
		{
		public:
			virtual ~FrameBuffer() = default;
			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			virtual void Resize(uint32_t width, uint32_t height, bool forceRecreate = false) = 0;

			virtual void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const = 0;

			virtual uint32_t GetWidth() const = 0;
			virtual uint32_t GetHeight() const = 0;

			virtual RendererID GetRendererID() const = 0;
			virtual RendererID GetColorAttachmentRendererID(int32_t index = 0) const = 0;
			virtual RendererID GetDepthAttachmentRendererID() const = 0;

			virtual const FrameBufferSpecification& GetSpecification() const = 0;

			static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
		};

		class VALHALLA_API FrameBufferPool final
		{
		public:
			FrameBufferPool(uint32_t maxFBs = 32);
			~FrameBufferPool();

			std::weak_ptr<FrameBuffer> AllocateBuffer();
			void Add(const Ref<FrameBuffer>& framebuffer);

			std::vector<Ref<FrameBuffer>>& GetAll() { return m_Pool; }
			const std::vector<Ref<FrameBuffer>>& GetAll() const { return m_Pool; }

			inline static FrameBufferPool* GetGlobal() { return s_Instance; }
		private:
			std::vector<Ref<FrameBuffer>> m_Pool;

			static FrameBufferPool* s_Instance;
		};
	}
}