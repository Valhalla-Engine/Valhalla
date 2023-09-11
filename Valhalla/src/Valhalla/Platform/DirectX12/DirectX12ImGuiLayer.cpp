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
		static bool show_demo_window = true;
		static bool show_another_window = true;

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

			Microsoft::WRL::ComPtr<ID3D12Device> device = DirectX12Context::GetCurrentDevice()->GetDevice();

			ImGui::StyleColorsDark();
			//SetDarkThemeColors();

			ImGui_ImplWin32_Init(static_cast<HWND>(Application::Get().GetWindow().GetNativeWindow()));
			ImGui_ImplDX12_Init(device.Get(), 2,
				DXGI_FORMAT_R8G8B8A8_UNORM, DirectX12Renderer::GetSRVHeap().Get(), DirectX12Renderer::GetSRVHeap().Get()->GetCPUDescriptorHandleForHeapStart(),
				DirectX12Renderer::GetSRVHeap().Get()->GetGPUDescriptorHandleForHeapStart());
			device->Release();
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
			ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectX12Renderer::GetCommandList().Get());

			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		void DirectX12ImGuiLayer::OnImGuiRender()
		{

		}
	}
}