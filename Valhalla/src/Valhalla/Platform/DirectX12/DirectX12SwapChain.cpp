#include "vhpch.h"
#include "DirectX12SwapChain.h"
#include "Valhalla/Core/Application.h"

#include "Valhalla/Renderer/Renderer.h"

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx12.h"

#include "DirectX12Context.h"

#include <codecvt>

namespace Valhalla
{
	namespace Yggdrasil
	{

		DirectX12SwapChain::DirectX12SwapChain()
			: m_Height(0), m_Width(0)
		{

		}

		DirectX12SwapChain::~DirectX12SwapChain()
		{
		}

		void DirectX12SwapChain::Destroy()
		{

		}

		void DirectX12SwapChain::Init(Ref<Heimdall::RendererContext>& context)
		{
			m_Device = context.As<DirectX12Context>()->GetDevice();
		}

		void DirectX12SwapChain::InitSurface()
		{

		}

		void DirectX12SwapChain::CreateSwapchain(uint32_t* width, uint32_t* height, bool vsync)
		{
			Microsoft::WRL::ComPtr<ID3D12Device> device = DirectX12Context::GetCurrentDevice()->GetDevice();

			m_Width = *width;
			m_Height = *height;
			m_VSync = vsync;

			VH_CORE_INFO("SwapChain Size: ({0}, {1})", m_Width, m_Height);

			D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevel{};
			QualityLevel.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			QualityLevel.SampleCount = 4;
			QualityLevel.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
			QualityLevel.NumQualityLevels = 0;
			DXCALL(device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
				&QualityLevel, sizeof(QualityLevel)));

			m_4xMSAAQuality = QualityLevel.NumQualityLevels;

			m_SwapChain.Reset();

			HWND window = static_cast<HWND>(Application::Get().GetWindow().GetNativeWindow());

			m_ColorFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

			DXGI_SWAP_CHAIN_DESC1 sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.BufferCount = 2;
			sd.Width = m_Width;
			sd.Height = m_Height;
			sd.Format = m_ColorFormat;
			sd.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			sd.Scaling = DXGI_SCALING_STRETCH;
			sd.Stereo = FALSE;


			{
				IDXGIFactory4* dxgiFactory = NULL;
				IDXGISwapChain1* swapChain1 = NULL;
				DXCALL(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));

				DXCALL(dxgiFactory->CreateSwapChainForHwnd(DirectX12Context::GetCurrentDevice()->GetGraphicsQueue().Get(), window, &sd, NULL, NULL, &swapChain1));
				DXCALL(swapChain1->QueryInterface(IID_PPV_ARGS(&m_SwapChain)));
				swapChain1->Release();
				dxgiFactory->Release();
				m_SwapChain->SetMaximumFrameLatency(2);
			}

			DXGI_SWAP_CHAIN_DESC1 temp;
			m_SwapChain->GetDesc1(&temp);
			m_ImageCount = temp.BufferCount;

			{
				D3D12_DESCRIPTOR_HEAP_DESC desc = {};
				desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				desc.NumDescriptors = m_ImageCount;
				desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
				desc.NodeMask = 1;
				DXCALL(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_RTVHeap)));

				SIZE_T rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
				D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_RTVHeap->GetCPUDescriptorHandleForHeapStart();
				m_RenderTargetViews.resize(m_ImageCount);
				for (UINT i = 0; i < m_ImageCount; i++)
				{
					m_RenderTargetViews[i] = rtvHandle;
					rtvHandle.ptr += rtvDescriptorSize;
				}
			}

			{
				D3D12_DESCRIPTOR_HEAP_DESC desc;
				desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				desc.NumDescriptors = 1;
				desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
				desc.NodeMask = 0;

				DXCALL(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_SRVHeap)));
			}

			m_RenderTargets.resize(m_ImageCount);

			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RTVHeap->GetCPUDescriptorHandleForHeapStart());
			for (size_t i = 0; i < m_ImageCount; i++)
			{
				m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_RenderTargets[i]));
				device->CreateRenderTargetView(m_RenderTargets[i].Get(), nullptr, rtvHandle);
				std::wstringstream ss;
				ss << i;
				std::wstring numStr = ss.str();
				std::wstring debugName = L"Back buffer: " + numStr;
				m_RenderTargets[i]->SetName(debugName.c_str());
				rtvHandle.Offset(1, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
			}

			// Create Command buffers
			{
				m_CommandBuffers.resize(m_ImageCount);
				for (size_t i = 0; i < m_CommandBuffers.size(); i++)
				{
					SwapChainCommandBuffer& commandBuffer = m_CommandBuffers[i];
					VH_CORE_INFO("Creating draw command buffers");
					DXCALL(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandBuffer.CommandAllocator)));
					std::wstringstream ss;
					ss << i;
					std::wstring numStr = ss.str();
					std::wstring commandName = L"command allocator: " + numStr;
					std::wstring listName = L"Command list: " + numStr;

					DXCALL(commandBuffer.CommandAllocator->SetName(commandName.c_str()));
					DXCALL(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandBuffer.CommandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandBuffer.CommandBuffer)));
					DXCALL(commandBuffer.CommandBuffer->Close());
					DXCALL(commandBuffer.CommandBuffer->SetName(listName.c_str()));
				}
			}

			{
				DXCALL(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));
				m_FenceValue = 1;

				// Create an event handle to use for frame synchronization.
				m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
				if (m_FenceEvent == nullptr)
				{
					DXCALL(HRESULT_FROM_WIN32(GetLastError()));
				}
			}

			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();(void)io;
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

			ImGui_ImplDX12_InitInfo init_info = {};
			init_info.Device = m_Device->GetDevice().Get();
			init_info.CommandQueue = m_Device->GetGraphicsQueue().Get();
			init_info.NumFramesInFlight = 2;
			init_info.RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			init_info.SrvDescriptorHeap = m_SRVHeap.Get();

			auto srvheap = m_SRVHeap;
			init_info.SrvDescriptorAllocFn = [](ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_handle)
			{
				auto swapchain = Application::Get().GetWindow().GetSwapChain().As<DirectX12SwapChain>();
				auto device = DirectX12Context::GetCurrentDevice()->GetDevice();
				uint32_t frameindex = swapchain->GetCurrentBackBufferIndex();
				out_cpu_handle->ptr = swapchain->GetSRVHeap()->GetCPUDescriptorHandleForHeapStart().ptr + (frameindex * device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
				out_gpu_handle->ptr = swapchain->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart().ptr + (frameindex * device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
			};

			init_info.SrvDescriptorFreeFn = [](ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle)
			{
				auto swapchain = Application::Get().GetWindow().GetSwapChain().As<DirectX12SwapChain>();
				auto device = DirectX12Context::GetCurrentDevice()->GetDevice();
				uint32_t handleIncrement = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
				int cpu_idx = (int)((cpu_handle.ptr - swapchain->GetSRVHeap()->GetCPUDescriptorHandleForHeapStart().ptr) / handleIncrement);
				int gpu_idx = (int)((gpu_handle.ptr - swapchain->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart().ptr) / handleIncrement);
				VH_CORE_ASSERT(cpu_idx == gpu_idx);
			};

			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX12_Init(&init_info);
		}

		void DirectX12SwapChain::BeginFrame()
		{
			//auto device = DirectX12Context::GetCurrentDevice();
			m_CurrentBufferIndex = m_SwapChain->GetCurrentBackBufferIndex();
			DXCALL(m_CommandBuffers[m_CurrentBufferIndex].CommandAllocator->Reset());
			DXCALL(m_CommandBuffers[m_CurrentBufferIndex].CommandBuffer->Reset(m_CommandBuffers[m_CurrentBufferIndex].CommandAllocator.Get(), nullptr));
			D3D12_CPU_DESCRIPTOR_HANDLE rtv = m_RenderTargetViews[m_CurrentBufferIndex];

			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			{
				static float f = 0.0f;
				static int counter = 0;
				static float color = 0;
				bool test = false;

				ImGui::Begin("Hello World!");

				ImGui::Text("this is some useful text");
				ImGui::Checkbox("Demo Window", &test);
				
				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
				ImGui::ColorEdit3("Clear color", (float*) &m_ClearColor);

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

		void DirectX12SwapChain::Present()
		{
			ImGui::Render();

			UINT backbufferindex = m_SwapChain->GetCurrentBackBufferIndex();

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> list = GetCurrentDrawCommandBuffer();

			D3D12_RESOURCE_BARRIER barrier	= {};
			barrier.Type					= D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Transition.pResource	= m_RenderTargets[backbufferindex].Get();
			barrier.Transition.Subresource	= D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			barrier.Transition.StateBefore	= D3D12_RESOURCE_STATE_PRESENT;
			barrier.Transition.StateAfter	= D3D12_RESOURCE_STATE_RENDER_TARGET;
			list->ResourceBarrier(1, &barrier);

			const float clear_color_with_alpha[4] = {m_ClearColor[0] * m_ClearColor[3], m_ClearColor[1] * m_ClearColor[3], m_ClearColor[2] * m_ClearColor[3], m_ClearColor[3]};
			list->ClearRenderTargetView(m_RenderTargetViews[backbufferindex], clear_color_with_alpha, 0, nullptr);
			list->OMSetRenderTargets(1, &m_RenderTargetViews[backbufferindex], false, nullptr);
			list->SetDescriptorHeaps(1, m_SRVHeap.GetAddressOf());

			ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), list.Get());

			barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
			barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
			list->ResourceBarrier(1, &barrier);

			DXCALL(list->Close());

			ID3D12CommandList* commandlist[] = { list.Get() };
			m_Device->GetGraphicsQueue()->ExecuteCommandLists(1, commandlist);

			ImGuiIO io = ImGui::GetIO(); (void)io;
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}
			
			DXCALL(m_SwapChain->Present(1, 0));
			DXCALL(m_Device->GetGraphicsQueue()->Signal(m_Fence.Get(), m_FenceValue));
			DXCALL(m_Device->GetDevice()->GetDeviceRemovedReason());
			WaitForLastSubmittedFrame();
		}


		void DirectX12SwapChain::WaitForLastSubmittedFrame()
		{
			// Signal and increment the fence value.
			const UINT64 fence = m_FenceValue;
			DXCALL(m_Device->GetGraphicsQueue()->Signal(m_Fence.Get(), fence));
			m_FenceValue++;

			// Wait until the previous frame is finished.
			if (m_Fence->GetCompletedValue() < fence)
			{

				DXCALL(m_Fence->SetEventOnCompletion(fence, m_FenceEvent));
				WaitForSingleObject(m_FenceEvent, INFINITE);
			}
		}

		void DirectX12SwapChain::OnResize(uint32_t width, uint32_t height)
		{
			VH_CORE_INFO("Waiting for last submitted frame");
			WaitForLastSubmittedFrame();
			VH_CORE_INFO("Done Waiting");

			for (uint32_t i = 0; i < m_RenderTargets.size(); i++)
			{
				m_RenderTargets[i].Reset();
			}


			DXCALL(m_SwapChain->ResizeBuffers(2, width, height, m_ColorFormat, DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT));

			for (size_t i = 0; i < m_ImageCount; i++)
			{
				m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_RenderTargets[i]));
				m_Device->GetDevice()->CreateRenderTargetView(m_RenderTargets[i].Get(), nullptr, m_RenderTargetViews[i]);
			}
		}

	}
}