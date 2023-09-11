#pragma once

#include "Valhalla/Renderer/VertexBuffer.h"
#include "VD3D12.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		class DirectX12VertexBuffer : public Heimdall::VertexBuffer
		{
		public:

		private:
			Microsoft::WRL::ComPtr<ID3D12Resource> m_VertexBuffer;
		};
	}
}