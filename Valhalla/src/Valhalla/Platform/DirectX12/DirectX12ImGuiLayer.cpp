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
			/*IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;


			ImGui::StyleColorsDark();
			//SetDarkTheme2VColors();

			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, style.Colors[ImGuiCol_WindowBg].w);

			Application& app = Application::Get();
			HWND window = static_cast<HWND>(app.GetWindow().GetNativeWindow());
			Ref<DirectX12SwapChain> swapchain = Application::Get().GetWindow().GetSwapChain();

			auto context = DirectX12Context::Get();
			auto device = DirectX12Context::GetCurrentDevice()->GetDevice();

			uint32_t frameIndex = Heimdall::Renderer::RT_GetCurrentFrameIndex();

			uint32_t framesInFlight = Heimdall::Renderer::GetConfig().FramesInFlight;
			
			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX12_Init(device.Get(), framesInFlight,
					DXGI_FORMAT_R8G8B8A8_UNORM, swapchain->GetSRVHeap().Get(),
					swapchain->GetSRVHeap().Get()->GetCPUDescriptorHandleForHeapStart(),
					swapchain->GetSRVHeap().Get()->GetGPUDescriptorHandleForHeapStart());
*/

		}

		void DirectX12ImGuiLayer::OnDetach()
		{
			/*Heimdall::Renderer::Submit([]() 
			{
				ImGui_ImplDX12_Shutdown();
				ImGui_ImplWin32_Shutdown();
				ImGui::DestroyContext();
			});*/
		}

		void DirectX12ImGuiLayer::Begin()
		{
			/*ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();*/
		}

		void DirectX12ImGuiLayer::End()
		{
			/*static bool show = true;
			ImGui::ShowDemoWindow(&show);

			ImGui::Render();
			/ *Ref<DirectX12SwapChain> swapchain = Application::Get().GetWindow().GetSwapChain();
			auto device = DirectX12Context::GetCurrentDevice()->GetDevice();

			uint32_t width = swapchain->GetWidth();
			uint32_t height = swapchain->GetHeight();

			uint32_t frameIndex = swapchain->GetCurrentBackBufferIndex();

			auto drawBuffer = swapchain->GetCurrentDrawCommandBuffer();

			auto renderTarget = swapchain->GetCurrentRenderTaget().Get();

			D3D12_RESOURCE_BARRIER barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = renderTarget;
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
			drawBuffer->ResourceBarrier(1, &barrier);

			VH_CORE_INFO("Trying to set color");
			const float color[4] = {0.2f, 0.2f, 0.2f, 1.0f};
			D3D12_CPU_DESCRIPTOR_HANDLE rtv = swapchain->GetCurrentRenderTagetView();
			drawBuffer->ClearRenderTargetView(rtv, color, 0, nullptr);
			drawBuffer->OMSetRenderTargets(1, &rtv, false, nullptr);


			//ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), drawBuffer.Get());

			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
			drawBuffer->ResourceBarrier(1, &barrier);* /
			
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}*/
		}

		void DirectX12ImGuiLayer::OnImGuiRender()
		{
			/*ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

			ImVec2 mousePos = ImGui::GetMousePos();

			ImGui::Text("Mouse Position: (%.1f, %.1f)", mousePos.x, mousePos.y);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End(); */               
		}
	}
}