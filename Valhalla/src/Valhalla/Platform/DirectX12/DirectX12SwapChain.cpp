#include "vhpch.h"
#include "DirectX12SwapChain.h"
#include "Valhalla/Core/Application.h"

#include "Valhalla/Renderer/Renderer.h"

#include "DirectX12Context.h"

namespace Valhalla
{
	namespace Yggdrasil
	{

		DirectX12SwapChain::DirectX12SwapChain()
		{

		}

		DirectX12SwapChain::~DirectX12SwapChain()
		{
			m_SwapChain->Release();
		}

		void DirectX12SwapChain::Init()
		{
			Microsoft::WRL::ComPtr<ID3D12Device> device = DirectX12Context::GetCurrentDevice()->GetDevice();

			D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevel;
			QualityLevel.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			QualityLevel.SampleCount = 4;
			QualityLevel.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
			QualityLevel.NumQualityLevels = 0;
			DXCALL(device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
				&QualityLevel, sizeof(QualityLevel)));

			device->Release();

			m_4xMSAAQuality = QualityLevel.NumQualityLevels;

			m_SwapChain.Reset();

			HWND window = static_cast<HWND>(Application::Get().GetWindow().GetNativeWindow());

			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
			swapChainDesc.BufferCount = 2;
			swapChainDesc.Width = Application::Get().GetWindow().GetWidth();
			swapChainDesc.Height = Application::Get().GetWindow().GetHeight();
			swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDesc.SampleDesc.Count = 1;
			Microsoft::WRL::ComPtr<IDXGIFactory4> factory = nullptr;
			DXCALL(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&factory)));

			m_ImageCount = swapChainDesc.BufferCount;

			Microsoft::WRL::ComPtr<IDXGISwapChain1> swap;
			DXCALL(factory->CreateSwapChainForHwnd(DirectX12Context::GetCurrentDevice()->GetGraphicsQueue().Get(), window, &swapChainDesc, nullptr, nullptr, &swap));
			factory->Release();
			DXCALL(swap.As(&m_SwapChain));
		}

		void DirectX12SwapChain::Present()
		{
			m_SwapChain->Present(0, 0);
		}

		void DirectX12SwapChain::BeginFrame()
		{

		}

		void DirectX12SwapChain::OnResize(uint32_t width, uint32_t height)
		{

		}

	}
}