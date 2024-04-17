#include "vhpch.h"
#include "DirectX12ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx12.h"

#include "Valhalla/Core/Application.h"

#include "DirectX12Context.h"
#include "DirectX12Renderer.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		static std::vector<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7>> s_ImGuiCommandBuffers;

		DirectX12ImGuiLayer::DirectX12ImGuiLayer()
		{

		}

		DirectX12ImGuiLayer::DirectX12ImGuiLayer(const std::string& name)
		{

		}

		DirectX12ImGuiLayer::~DirectX12ImGuiLayer()
		{

		}

		void DirectX12ImGuiLayer::OnAttach()
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;


			ImGui::StyleColorsDark();
			SetDarkTheme2VColors();

			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, style.Colors[ImGuiCol_WindowBg].w);

			Application& app = Application::Get();
			HWND window = static_cast<HWND>(app.GetWindow().GetNativeWindow());
			ImGui_ImplWin32_Init(window);

			DirectX12ImGuiLayer* instance = this;
			Heimdall::Renderer::Submit([instance]()
			{
				Application& app = Application::Get();

				auto context = DirectX12Context::Get();
				auto device = DirectX12Context::GetCurrentDevice()->GetDevice();

				uint32_t frameIndex = Heimdall::Renderer::RT_GetCurrentFrameIndex();

				uint32_t framesInFlight = Heimdall::Renderer::GetConfig().FramesInFlight;

				D3D12_DESCRIPTOR_HEAP_DESC RTVHeapDesc;
				RTVHeapDesc.NumDescriptors = 1;
				RTVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				RTVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

				D3D12_DESCRIPTOR_HEAP_DESC DSVHeapDesc;
				DSVHeapDesc.NumDescriptors = 1;
				DSVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
				DSVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

				DXCALL(device->CreateDescriptorHeap(&RTVHeapDesc, IID_PPV_ARGS(instance->m_RTVHeap.GetAddressOf())));
				DXCALL(device->CreateDescriptorHeap(&DSVHeapDesc, IID_PPV_ARGS(instance->m_DSVHeap.GetAddressOf())));

				ImGui_ImplDX12_Init(device.Get(), framesInFlight,
					DXGI_FORMAT_R8G8B8A8_UNORM, instance->m_RTVHeap.Get(), instance->m_RTVHeap.Get()->GetCPUDescriptorHandleForHeapStart(),
					instance->m_RTVHeap.Get()->GetGPUDescriptorHandleForHeapStart());


				s_ImGuiCommandBuffers.resize(framesInFlight);
				for (size_t i = 0; i < framesInFlight; i++)
					s_ImGuiCommandBuffers[i] = DirectX12Context::GetCurrentDevice()->CreateSecondaryCommandBuffer("ImGuiSecondaryCommandBuffer");
			});
		}

		void DirectX12ImGuiLayer::OnDetach()
		{
			ImGui_ImplDX12_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
		}

		void DirectX12ImGuiLayer::Begin()
		{
			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
		}

		void DirectX12ImGuiLayer::End()
		{
			ImGui::Render();
			Ref<DirectX12SwapChain> swapchain = Application::Get().GetWindow().GetSwapChain();

			D3D12_CLEAR_VALUE clearValues[2];
			clearValues[0].Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			clearValues[0].Color[0] = 0.1f;
			clearValues[0].Color[1] = 0.1f;
			clearValues[0].Color[2] = 0.1f;
			clearValues[0].Color[3] = 1.0f;

			clearValues[1].Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			clearValues[1].DepthStencil = { 1.0f, 0 };

			uint32_t width = swapchain->GetWidth();
			uint32_t height = swapchain->GetHeight();

			uint32_t commandBufferindex = swapchain->GetCurrentBackBufferIndex();

			auto drawCommandBuffer = swapchain->GetCurrentDrawCommandBuffer();


			D3D12_RENDER_PASS_RENDER_TARGET_DESC renderTargetDesc;
			renderTargetDesc.cpuDescriptor = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_RTVHeap->GetCPUDescriptorHandleForHeapStart());
			renderTargetDesc.BeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
			renderTargetDesc.BeginningAccess.Clear.ClearValue = clearValues[0];
			renderTargetDesc.EndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;

			D3D12_RENDER_PASS_DEPTH_STENCIL_DESC depthStencilDesc;
			depthStencilDesc.cpuDescriptor = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_DSVHeap->GetCPUDescriptorHandleForHeapStart());
			depthStencilDesc.DepthBeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
			depthStencilDesc.DepthBeginningAccess.Clear.ClearValue = clearValues[1];
			depthStencilDesc.StencilBeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
			depthStencilDesc.StencilBeginningAccess.Clear.ClearValue = clearValues[1];
			depthStencilDesc.DepthEndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;
			depthStencilDesc.StencilEndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;

			drawCommandBuffer->BeginRenderPass(1, &renderTargetDesc, &depthStencilDesc, D3D12_RENDER_PASS_FLAG_NONE);

			D3D12_VIEWPORT viewport;
			viewport.TopLeftX = 0.0f;
			viewport.TopLeftY = (float)height;
			viewport.Height = -(float)height;
			viewport.Width = (float)width;
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;
			drawCommandBuffer->RSSetViewports(1, &viewport);

			D3D12_RECT rect;
			rect.left = 0;
			rect.top = 0;
			rect.right = width;
			rect.bottom = height;
			drawCommandBuffer->RSSetScissorRects(1, &rect);

			ImDrawData* main_draw_data = ImGui::GetDrawData();
			ImGui_ImplDX12_RenderDrawData(main_draw_data, drawCommandBuffer.Get());

			drawCommandBuffer->EndRenderPass();
			
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}
		}

		void DirectX12ImGuiLayer::OnImGuiRender()
		{

		}
	}
}