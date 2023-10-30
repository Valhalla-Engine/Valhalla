#include "vhpch.h"
#include "DirectX12FrameBuffer.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		DirectX12FrameBuffer::DirectX12FrameBuffer(const Heimdall::FrameBufferSpecification& spec)
		{

		}

		DirectX12FrameBuffer::~DirectX12FrameBuffer()
		{
		}

		void DirectX12FrameBuffer::Resize(uint32_t width, uint32_t height, bool forceRecreate)
		{

		}

		void DirectX12FrameBuffer::AddResizeCallback(const std::function<void(Ref<FrameBuffer>)>& func)
		{
			m_ResizeCallbacks.push_back(func);
		}
	}
}