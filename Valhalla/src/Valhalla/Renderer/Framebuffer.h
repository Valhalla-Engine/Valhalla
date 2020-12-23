#pragma once

#include <glm/glm.hpp>

#include "RendererAPI.h"

#include <vector>

namespace Valhalla
{
	namespace Heimdal
	{
		enum class VALHALLA_API FramebufferFormat
		{
			None = 0,
			RGBA8 = 1,
			RGBA16 = 2
		};

		struct VALHALLA_API	FramebufferSpecification
		{
			uint32_t Width = 1280;
			uint32_t Height = 720;
			glm::vec4 ClearColor;
			FramebufferFormat format = FramebufferFormat::RGBA8;
			uint32_t samples = 1; //multisampling

			// swapChainTarget = screen buffer (i.e no framebuffer)
			bool swapChainTarget = false;
		};

		class VALHALLA_API Framebuffer : public RefCounted
		{
		public:
			virtual ~Framebuffer() {}
			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			virtual void Resize(uint32_t width, uint32_t heigt, bool forceRecreate = false) = 0;

			virtual void BindTexture(uint32_t slot = 0) const = 0;

			virtual RendererID GetRendererID() const = 0;
			virtual RendererID GetColorAttachmentRendererID() const = 0;
			virtual RendererID GetDepthAttachmentRendererID() const = 0;

			virtual const FramebufferSpecification& GetSpecification() const = 0;

			static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
		};

		class VALHALLA_API FramebufferPool final
		{
		public:
			FramebufferPool(uint32_t maxFBs = 32);
			~FramebufferPool();

			std::weak_ptr<Framebuffer> AllocateBuffer();
			void Add(const Ref<Framebuffer>& framebuffer);

			std::vector<Ref<Framebuffer>>& GetAll() { return m_Pool; }
			const std::vector<Ref<Framebuffer>>& GetAll() const { return m_Pool; }

			inline static FramebufferPool* GetGlobal() { return s_Instance; }
		private:
			std::vector<Ref<Framebuffer>> m_Pool;

			static FramebufferPool* s_Instance;
		};
	}
}