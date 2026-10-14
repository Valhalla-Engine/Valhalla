module;
#include "D12Include.hpp"

export module Valhalla.Yggdrasil.DirectX12.Renderer;

import Valhalla.Renderer.RendererAPI;
import Valhalla.Yggdrasil.DirectX12.Device;
import Valhalla.Yggdrasil.DirectX12.SwapChain;
import Valhalla.Yggdrasil.DirectX12.FrameContext;

import Valhalla.Core.Ref;
import Valhalla.Math;

export namespace Valhalla
{
	export namespace Yggdrasil
	{
		class DirectX12Renderer : public Heimdall::RendererAPI
		{

		public:

			~DirectX12Renderer() = default;
			virtual void Init() override;
			virtual void Shutdown() override;

			virtual void BeginFrame() override;
			virtual void EndFrame() override;

			DirectX12FrameContext& GetCurrentFrameContext();
		private:
			std::vector<DirectX12FrameContext> m_FrameContext;

			DirectX12FrameContext* m_CurrentFrame = nullptr;

			uint64_t m_FrameNumber = 0;

			Ref<DirectX12Device> m_DirectX12Device;
			Ref<DirectX12SwapChain> m_DirectX12Swap;

			void LoadPipeline();
			void LoadAssets();
			void PopulateCommandList();
			void WaitForPreviousFrame();

			inline static DirectX12Renderer* instance;
		};
	}
}