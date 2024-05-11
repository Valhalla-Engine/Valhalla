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
			//SetDarkTheme2VColors();

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
				Ref<DirectX12SwapChain> swapchain = Application::Get().GetWindow().GetSwapChain();

				auto context = DirectX12Context::Get();
				auto device = DirectX12Context::GetCurrentDevice()->GetDevice();

				uint32_t frameIndex = Heimdall::Renderer::RT_GetCurrentFrameIndex();

				uint32_t framesInFlight = Heimdall::Renderer::GetConfig().FramesInFlight;

				{
					D3D12_DESCRIPTOR_HEAP_DESC desc = {};
					desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
					desc.NumDescriptors = framesInFlight;
					desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
					desc.NodeMask = 1;

					DXCALL(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&instance->m_RTVHeap)));

					SIZE_T rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
					D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = instance->m_RTVHeap->GetCPUDescriptorHandleForHeapStart();
					instance->m_RenderTargetDescriptors.resize(framesInFlight);
					for (UINT i = 0; i < framesInFlight; i++)
					{
						instance->m_RenderTargetDescriptors[i] = rtvHandle;
						rtvHandle.ptr += rtvDescriptorSize;
					}
				}

				{
					D3D12_DESCRIPTOR_HEAP_DESC desc = {};
					desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
					desc.NumDescriptors = 1;
					desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
					DXCALL(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&instance->m_SRVHeap)))
				}

				{
					CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(instance->m_RTVHeap->GetCPUDescriptorHandleForHeapStart());
					instance->m_RenderTargets.resize(framesInFlight);
					for (size_t i = 0; i < instance->m_RenderTargets.size(); i++)
					{
						ID3D12Resource* pBackBuffer = NULL;
						swapchain->GetSwapChain()->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
						device->CreateRenderTargetView(pBackBuffer, NULL, instance->m_RenderTargetDescriptors[i]);
						instance->m_RenderTargets[i] = pBackBuffer;
					}
				}
				
				ImGui_ImplWin32_Init(window);
				ImGui_ImplDX12_Init(device.Get(), framesInFlight,
					DXGI_FORMAT_R8G8B8A8_UNORM, instance->m_SRVHeap.Get(), 
					instance->m_SRVHeap.Get()->GetCPUDescriptorHandleForHeapStart(),
					instance->m_SRVHeap.Get()->GetGPUDescriptorHandleForHeapStart());

			});
		}

		void DirectX12ImGuiLayer::OnDetach()
		{
			Heimdall::Renderer::Submit([]() 
			{
				ImGui_ImplDX12_Shutdown();
				ImGui_ImplWin32_Shutdown();
				ImGui::DestroyContext();
			});
		}

		void DirectX12ImGuiLayer::Begin()
		{
			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();


		}

		void DirectX12ImGuiLayer::End()
		{
			static bool show = true;
			ImGui::ShowDemoWindow(&show);

			ImGui::Render();
			Ref<DirectX12SwapChain> swapchain = Application::Get().GetWindow().GetSwapChain();
			auto device = DirectX12Context::GetCurrentDevice()->GetDevice();

			uint32_t width = swapchain->GetWidth();
			uint32_t height = swapchain->GetHeight();

			uint32_t frameIndex = swapchain->GetCurrentBackBufferIndex();

			auto drawBuffer = swapchain->GetCurrentDrawCommandBuffer();

			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RTVHeap->GetCPUDescriptorHandleForHeapStart());
			rtvHandle.Offset(frameIndex, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));


			D3D12_RESOURCE_BARRIER barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = m_RenderTargets[frameIndex].Get();
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
			drawBuffer->ResourceBarrier(1, &barrier);

			// Render Dear ImGui graphics
			const float clear_color_with_alpha[4] = { 0.2f, 0.2f, 0.2f, 1.0f };	
			drawBuffer->ClearRenderTargetView(m_RenderTargetDescriptors[frameIndex], clear_color_with_alpha, 0, NULL);
			drawBuffer->OMSetRenderTargets(1, &m_RenderTargetDescriptors[frameIndex], FALSE, NULL);
			ID3D12DescriptorHeap* descriptorHeaps[] = { m_SRVHeap.Get() };
			drawBuffer->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

			ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), drawBuffer.Get());
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
			drawBuffer->ResourceBarrier(1, &barrier);
			
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault(nullptr, drawBuffer.Get());
			}
		}

		void DirectX12ImGuiLayer::OnImGuiRender()
		{
			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

			ImVec2 mousePos = ImGui::GetMousePos();

			ImGui::Text("Mouse Position: (%.1f, %.1f)", mousePos.x, mousePos.y);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();                
		}
	}
}