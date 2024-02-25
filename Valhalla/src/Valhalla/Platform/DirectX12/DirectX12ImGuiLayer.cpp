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
		static std::vector<Microsoft::WRL::ComPtr<ID3D12CommandList>> s_ImGuiCommandBuffers;

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

			DirectX12ImGuiLayer* instance = this;
			Heimdall::Renderer::Submit([instance]()
			{
				Application& app = Application::Get();
				HWND window = static_cast<HWND>(app.GetWindow().GetNativeWindow());

				auto context = DirectX12Context::Get();
				auto device = DirectX12Context::GetCurrentDevice()->GetDevice();

				uint32_t frameIndex = Heimdall::Renderer::RT_GetCurrentFrameIndex();

				ImGui_ImplWin32_Init(window);
				ImGui_ImplDX12_Init(device.Get(), 2,
					DXGI_FORMAT_R8G8B8A8_UNORM, DirectX12Renderer::GetFrameHeaps(frameIndex).SRVHeap.Get(), DirectX12Renderer::GetFrameHeaps(frameIndex).SRVHeap.Get()->GetCPUDescriptorHandleForHeapStart(),
					DirectX12Renderer::GetFrameHeaps(frameIndex).SRVHeap.Get()->GetGPUDescriptorHandleForHeapStart());

				uint32_t framesInFlight = Heimdall::Renderer::GetConfig().FramesInFlight;
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
			auto drawdata = ImGui::GetDrawData();
			drawdata->DisplaySize.x = Application::Get().GetWindow().GetWidth();
			drawdata->DisplaySize.y = Application::Get().GetWindow().GetHeight();
			//ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectX12Renderer::GetCommandList().Get());

			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		void DirectX12ImGuiLayer::OnImGuiRender()
		{

		}
	}
}