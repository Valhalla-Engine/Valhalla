#pragma once
#include "VD3D12.h"

#include "Valhalla/Renderer/SwapChain.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		class DirectX12SwapChain : public Heimdall::SwapChain
		{
		public:
			DirectX12SwapChain();
			virtual ~DirectX12SwapChain();

			virtual void Init() override;

			virtual void Present() override;
			virtual void BeginFrame() override;
			virtual void OnResize(uint32_t width, uint32_t height) override;

			Microsoft::WRL::ComPtr<IDXGISwapChain3> GetSwapChain() { return m_SwapChain; }
		private:
			Microsoft::WRL::ComPtr<IDXGISwapChain3> m_SwapChain;
			uint32_t m_4xMSAAQuality;
		};
	}
}