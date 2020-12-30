#include "vhpch.h"
#include "FrameBuffer.h"

#include "Valhalla/Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Valhalla
{
	namespace Heimdal
	{
		Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
		{
			Ref<FrameBuffer> result = nullptr;

			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:		result = nullptr;
				case RendererAPIType::OpenGL:	result = Ref<Yggdrasil::OpenGLFrameBuffer>::Create(spec);
			}
			FrameBufferPool::GetGlobal()->Add(result);
			return result;
		}

		FrameBufferPool* FrameBufferPool::s_Instance = new FrameBufferPool;

		FrameBufferPool::FrameBufferPool(uint32_t maxFBs)
		{

		}

		FrameBufferPool::~FrameBufferPool()
		{

		}

		std::weak_ptr<FrameBuffer> FrameBufferPool::AllocateBuffer()
		{
			return std::weak_ptr<FrameBuffer>();
		}

		void FrameBufferPool::Add(const Ref<FrameBuffer>& framebuffer)
		{
			m_Pool.push_back(framebuffer);
		}
	}
}