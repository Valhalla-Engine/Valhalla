#include "vhpch.h"
#include "Framebuffer.h"

namespace Valhalla
{
	namespace Heimdal
	{
		Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
		{
			Ref<Framebuffer> result = nullptr;

			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:				return nullptr;
				case RendererAPIType::OpenGL:			result = nullptr; break;
				case RendererAPIType::DirectX11:		VH_CORE_ASSERT(false, "DirectX 11 is currently not supported"); result = nullptr; break;
				case RendererAPIType::DirectX12:		VH_CORE_ASSERT(false, "DirectX 12 is currently not supported"); result = nullptr; break;
				case RendererAPIType::Vulkan:			VH_CORE_ASSERT(false, "Vulkan is currently not supported"); result = nullptr; break;
				case RendererAPIType::Metal:			VH_CORE_ASSERT(false, "Metal is currently not supported"); result = nullptr; break;
			}

			FramebufferPool::GetGlobal()->Add(result);
			return result;
		}

		FramebufferPool* FramebufferPool::s_Instance = new FramebufferPool;

		FramebufferPool::FramebufferPool(uint32_t maxFBs)
		{

		}

		FramebufferPool::~FramebufferPool()
		{

		}

		std::weak_ptr<Framebuffer> FramebufferPool::AllocateBuffer()
		{
			return std::weak_ptr<Framebuffer>();
		}

		void FramebufferPool::Add(const Ref<Framebuffer>& framebuffer)
		{
			m_Pool.push_back(framebuffer);
		}
	}
}