module;
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx12.h"

#include "D12Include.hpp"

module Valhalla.Yggdrasil.DirectX12.ImguiLayer;

import Valhalla.Yggdrasil.DirectX12.Context;
import Valhalla.Yggdrasil.DirectX12.Renderer;
import Valhalla.Yggdrasil.DirectX12.SwapChain;

import Valhalla.Core.Application;
import Valhalla.Core.Log;
import Valhalla.Core.Assert;

import Valhalla.Renderer.Renderer;

import <vector>;

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
			Application& app = Application::Get();
			HWND window = static_cast<HWND>(app.GetWindow().GetNativeWindow());
			Ref<DirectX12SwapChain> swapchain = Application::Get().GetWindow().GetSwapChain();

			auto context = DirectX12Context::Get();
			auto device = DirectX12Context::GetCurrentDevice();

			uint32_t frameIndex = Heimdall::Renderer::RT_GetCurrentFrameIndex();

			uint32_t framesInFlight = Heimdall::Renderer::GetConfig().FramesInFlight;

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

			ImGui::StyleColorsDark();
			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			m_SRVHeap = Application::Get().GetWindow().GetSwapChain().As<DirectX12SwapChain>()->GetSRVHeap();
			

			ImGui_ImplDX12_InitInfo init_info = {};
			init_info.Device = device->GetDevice().Get();
			init_info.CommandQueue = device->GetGraphicsQueue().Get();
			init_info.NumFramesInFlight = framesInFlight;
			init_info.RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			init_info.SrvDescriptorHeap = Application::Get().GetWindow().GetSwapChain().As<DirectX12SwapChain>()->GetSRVHeap().Get();
			init_info.UserData = (void*)this;


			auto layer = this;
			init_info.SrvDescriptorAllocFn = [](ImGui_ImplDX12_InitInfo* info, D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_handle)
			{
				 auto layer = static_cast<DirectX12ImGuiLayer*>(info->UserData);
				 return layer->AllocSRVHeap(out_cpu_handle, out_gpu_handle);
			};

			init_info.SrvDescriptorFreeFn = [](ImGui_ImplDX12_InitInfo* info, D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle)
			{
				auto layer = static_cast<DirectX12ImGuiLayer*>(info->UserData);
				return layer->FreeSRVHeap(cpu_handle, gpu_handle);
			};
			
			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX12_Init(&init_info);
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
			ImGui_ImplWin32_NewFrame();
			ImGui_ImplDX12_NewFrame();
			ImGui::NewFrame();
		}

		void DirectX12ImGuiLayer::End()
		{
			ImGui::Render();

			auto swapchain = Application::Get().GetWindow().GetSwapChain().As<DirectX12SwapChain>();
			swapchain->SetBackgroundColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);

			UINT backbufferindex = swapchain->GetCurrentBackBufferIndex();

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> list = swapchain->GetCurrentDrawCommandBuffer();

			auto renderTarget = swapchain->GetCurrentRenderTaget();

			D3D12_RESOURCE_BARRIER barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Transition.pResource = renderTarget.Get();
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
			list->ResourceBarrier(1, &barrier);
			ID3D12DescriptorHeap* heaps[] = {m_SRVHeap.Get()};
			list->SetDescriptorHeaps(1, heaps);

			ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), list.Get());

			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
			list->ResourceBarrier(1, &barrier);

			ImGuiIO io = ImGui::GetIO(); (void)io;
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}
		}

		void DirectX12ImGuiLayer::OnImGuiRender()
		{
			{
				static float f = 0.0f;
				static int counter = 0;
				static float color = 0;
				bool test = false;

				ImGui::Begin("Hello World!");

				ImGui::Text("this is some useful text");
				ImGui::Checkbox("Demo Window", &test);

				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
				ImGui::ColorEdit3("Clear color", (float*)&m_ClearColor);


				if (ImGui::Button("Button"))
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

				ImVec2 mousePos = ImGui::GetMousePos();

				ImGui::Text("Moise position: (%.1f, %.1f)", mousePos.x, mousePos.y);

				ImGui::End();
			}
		}

		void DirectX12ImGuiLayer::AllocSRVHeap(D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_desc_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_desc_handle)
		{
			uint32_t frameIndex = Application::Get().GetWindow().GetSwapChain().As<DirectX12SwapChain>()->GetCurrentBackBufferIndex();
			auto srvheap = Application::Get().GetWindow().GetSwapChain().As<DirectX12SwapChain>()->GetSRVHeap();
			auto device = DirectX12Context::GetCurrentDevice()->GetDevice();

			out_cpu_desc_handle->ptr = srvheap->GetCPUDescriptorHandleForHeapStart().ptr + (frameIndex * device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
			out_gpu_desc_handle->ptr = srvheap->GetGPUDescriptorHandleForHeapStart().ptr + (frameIndex * device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		}

		void DirectX12ImGuiLayer::FreeSRVHeap(D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle)
		{
			auto device = DirectX12Context::GetCurrentDevice()->GetDevice();
			auto srvheap = Application::Get().GetWindow().GetSwapChain().As<DirectX12SwapChain>()->GetSRVHeap();
			uint32_t handleIncrement = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			int cpu_idx = (int)((cpu_handle.ptr - srvheap->GetCPUDescriptorHandleForHeapStart().ptr) / handleIncrement);
			int gpu_idx = (int)((gpu_handle.ptr - srvheap->GetGPUDescriptorHandleForHeapStart().ptr) / handleIncrement);
			Assert(cpu_idx == gpu_idx);
		}
	}
}