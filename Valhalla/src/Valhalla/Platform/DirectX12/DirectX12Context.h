#pragma once
#include "VD3D12.h"
#include "Valhalla/Renderer/RendererContext.h"
#include "DirectX12Device.h"
#include "DirectX12SwapChain.h"
#include "Valhalla/Renderer/Renderer.h"
#include <d3dcompiler.h>

namespace Valhalla
{
	namespace Yggdrasil
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 Position;
			DirectX::XMFLOAT4 Color;
		};

		struct VertexPosition
		{
			DirectX::XMFLOAT3 Position;
		};

		class DirectX12Context : public Heimdall::RendererContext
		{
		public:
			DirectX12Context();
			virtual ~DirectX12Context() override;

			virtual void Init() override;

			Ref<DirectX12Device> GetDevice() { return m_Device; }
			Ref<DirectX12SwapChain> GetSwapChain() { return m_SwapChain; }

			static Ref<DirectX12Context> Get() { return Ref<DirectX12Context>(Heimdall::Renderer::GetContext()); }
			static Ref<DirectX12Device> GetCurrentDevice() { return Get()->GetDevice(); }
			static Ref<DirectX12SwapChain> GetCurrentSwapChain() { return Get()->GetSwapChain(); }

		private:
			Ref<DirectX12Device> m_Device;
			Ref<DirectX12SwapChain> m_SwapChain;
		};
	}
}