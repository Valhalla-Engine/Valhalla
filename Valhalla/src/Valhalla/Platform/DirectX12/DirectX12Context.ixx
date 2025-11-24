#include <DirectXMath.h>

export module Valhalla.Yggdrasil.DirectX12.Context;

import Valhalla.Yggdrasil.DirectX12.Device;
import Valhalla.Yggdrasil.DirectX12.SwapChain;

import Valhalla.Renderer.RendererContext;
import Valhalla.Renderer.Renderer;

import <compare>;

export namespace Valhalla
{
	export namespace Yggdrasil
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

		private:
			Ref<DirectX12Device> m_Device;
			Ref<DirectX12SwapChain> m_SwapChain;
		};
	}
}