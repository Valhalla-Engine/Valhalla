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

				D3D12_DESCRIPTOR_HEAP_DESC RTVHeapDesc;
				RTVHeapDesc.NumDescriptors = framesInFlight;
				RTVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				RTVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				RTVHeapDesc.NodeMask = 0;

				D3D12_DESCRIPTOR_HEAP_DESC DSVHeapDesc;
				DSVHeapDesc.NumDescriptors = framesInFlight;
				DSVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
				DSVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				DSVHeapDesc.NodeMask = 0;

				D3D12_DESCRIPTOR_HEAP_DESC SRVHeapDesc;
				SRVHeapDesc.NumDescriptors = framesInFlight;
				SRVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				SRVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
				SRVHeapDesc.NodeMask = 0;

				DXCALL(device->CreateDescriptorHeap(&RTVHeapDesc, IID_PPV_ARGS(instance->m_RTVHeap.GetAddressOf())));
				DXCALL(device->CreateDescriptorHeap(&DSVHeapDesc, IID_PPV_ARGS(instance->m_DSVHeap.GetAddressOf())));
				DXCALL(device->CreateDescriptorHeap(&SRVHeapDesc, IID_PPV_ARGS(instance->m_SRVHeap.GetAddressOf())));

				uint32_t rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

				CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(instance->m_RTVHeap->GetCPUDescriptorHandleForHeapStart());

				D3D12_RENDER_TARGET_VIEW_DESC RTVDesc;
				RTVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
				RTVDesc.Texture2D.MipSlice = 0;
				RTVDesc.Texture2D.PlaneSlice = 0;


				instance->m_RenderTargets.resize(framesInFlight);
				for (uint32_t i = 0; i < instance->m_RenderTargets.size(); i++)
				{
					DXCALL(swapchain->GetSwapChain()->GetBuffer(i, IID_PPV_ARGS(&instance->m_RenderTargets[i])));
					device->CreateRenderTargetView(instance->m_RenderTargets[i].Get(), &RTVDesc, rtvHandle);
					rtvHandle.Offset(1, rtvDescriptorSize);
				}

				uint32_t dsvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
				CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(instance->m_DSVHeap->GetCPUDescriptorHandleForHeapStart());

				D3D12_RESOURCE_DESC depthStencilDesc;
				depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
				depthStencilDesc.Alignment = 0;
				depthStencilDesc.Width = swapchain->GetWidth();
				depthStencilDesc.Height = swapchain->GetHeight();
				depthStencilDesc.DepthOrArraySize = 1;
				depthStencilDesc.MipLevels = 1;
				depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				depthStencilDesc.SampleDesc.Count = 1;
				depthStencilDesc.SampleDesc.Quality = 0;
				depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
				depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

				D3D12_CLEAR_VALUE optClear;
				optClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				optClear.DepthStencil.Depth = 1.0f;
				optClear.DepthStencil.Stencil = 0;

				instance->m_DepthStencilBuffer.resize(framesInFlight);
				for (uint32_t i = 0; i < instance->m_DepthStencilBuffer.size(); i++)
				{
					CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_DEFAULT);
					DXCALL(device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &depthStencilDesc, D3D12_RESOURCE_STATE_COMMON, 
						&optClear, IID_PPV_ARGS(&instance->m_DepthStencilBuffer[i])));
					device->CreateDepthStencilView(instance->m_DepthStencilBuffer[i].Get(), nullptr, dsvHandle);
					dsvHandle.Offset(1, dsvDescriptorSize);
				}
				
				ImGui_ImplWin32_Init(window);
				ImGui_ImplDX12_Init(device.Get(), framesInFlight,
					DXGI_FORMAT_R8G8B8A8_UNORM, instance->m_SRVHeap.Get(), instance->m_SRVHeap.Get()->GetCPUDescriptorHandleForHeapStart(),
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
			ImGui::Render();
			Ref<DirectX12SwapChain> swapchain = Application::Get().GetWindow().GetSwapChain();
			auto device = DirectX12Context::GetCurrentDevice()->GetDevice();

			uint32_t width = swapchain->GetWidth();
			uint32_t height = swapchain->GetHeight();

			ImGui::Render();

			D3D12_CLEAR_VALUE clearValues[2];
			clearValues[0].Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			clearValues[0].Color[0] = 0.1f;
			clearValues[0].Color[1] = 0.1f;
			clearValues[0].Color[2] = 0.1f;
			clearValues[0].Color[3] = 1.0f;

			clearValues[1].Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			clearValues[1].DepthStencil = { 1.0f, 0 };


			uint32_t commandBufferindex = swapchain->GetCurrentBackBufferIndex();

			auto drawCommandBuffer = swapchain->GetCurrentDrawCommandBuffer();

			uint32_t rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			uint32_t dsvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RTVHeap->GetCPUDescriptorHandleForHeapStart());
			rtvHandle.Offset(commandBufferindex, rtvDescriptorSize);

			CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_DSVHeap->GetCPUDescriptorHandleForHeapStart());
			dsvHandle.Offset(commandBufferindex, dsvDescriptorSize);

			ID3D12DescriptorHeap* descriptorHeaps[] = { m_SRVHeap.Get() };
			drawCommandBuffer->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);


			CD3DX12_RESOURCE_BARRIER resourceBarrierDepth;
			resourceBarrierDepth = resourceBarrierDepth.Transition(m_DepthStencilBuffer[commandBufferindex].Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
			drawCommandBuffer->ResourceBarrier(1, &resourceBarrierDepth);

			CD3DX12_RESOURCE_BARRIER resourceBarrierRT;
			resourceBarrierRT = resourceBarrierRT.Transition(m_RenderTargets[commandBufferindex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

			// Define the clear color (RGBA)
			float clearColor[] = { 0.0f, 0.0f, 1.0f, 1.0f }; // Blue color with full alpha

			// Transition the render target from its current state to the render target state
			drawCommandBuffer->ResourceBarrier(1, &resourceBarrierRT);

			// Clear the render target view to the specified clear color
			drawCommandBuffer->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

			// Transition the render target back to the present state
			drawCommandBuffer->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

			/*D3D12_VIEWPORT viewport = {};
			viewport.TopLeftX = (float)0.0f;
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
			drawCommandBuffer->RSSetScissorRects(1, &rect);*/

			ImDrawData* main_draw_data = ImGui::GetDrawData();
			ImGui_ImplDX12_RenderDrawData(main_draw_data, drawCommandBuffer.Get());

			/*resourceBarrierDepth = resourceBarrierDepth.Transition(m_DepthStencilBuffer[commandBufferindex].Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_COMMON);
			drawCommandBuffer->ResourceBarrier(1, &resourceBarrierDepth);
			resourceBarrierRT.Transition(m_RenderTargets[commandBufferindex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
			drawCommandBuffer->ResourceBarrier(1, &resourceBarrierRT);*/
			
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}
		}

		void DirectX12ImGuiLayer::OnImGuiRender()
		{
			ImGui::ShowDemoWindow(nullptr);
		}
	}
}