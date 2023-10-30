#include "vhpch.h"
#include "DirectX12Renderer.h"
#include "Valhalla/Core/Application.h"

#include "DirectX12Context.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		struct DirectX12RendererData
		{
			std::vector<DirectX12DescriptorHeaps> DescriptorHeaps;
		};

		static DirectX12RendererData* s_Data;

		void DirectX12Renderer::Init()
		{
			s_Data = new DirectX12RendererData();

			const auto& config = Heimdall::Renderer::GetConfig();
			s_Data->DescriptorHeaps.resize(config.FramesInFlight);

			Heimdall::Renderer::Submit([]() mutable
			{
				auto device = DirectX12Context::GetCurrentDevice();

				D3D12_DESCRIPTOR_HEAP_DESC RTVHeapDesc;
				RTVHeapDesc.NumDescriptors = 1000;
				RTVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
				RTVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

				D3D12_DESCRIPTOR_HEAP_DESC SRVHeapDesc;
				SRVHeapDesc.NumDescriptors = 1000;
				SRVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				SRVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
				
				D3D12_DESCRIPTOR_HEAP_DESC CBVHeapDesc;
				CBVHeapDesc.NumDescriptors = 1000;
				CBVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				CBVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

				D3D12_DESCRIPTOR_HEAP_DESC UAVHeapDesc;
				UAVHeapDesc.NumDescriptors = 1000;
				UAVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
				UAVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

				D3D12_DESCRIPTOR_HEAP_DESC SamplerHeapDesc;
				SamplerHeapDesc.NumDescriptors = 1000;
				SamplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
				SamplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

				D3D12_DESCRIPTOR_HEAP_DESC DSVHeapDesc;
				DSVHeapDesc.NumDescriptors = 1000;
				DSVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
				DSVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

				VH_CORE_INFO("Creating DescriptorHeaps");

				for (size_t i = 0; i < s_Data->DescriptorHeaps.size(); i++)
				{
					device->GetDevice()->CreateDescriptorHeap(&RTVHeapDesc, IID_PPV_ARGS(s_Data->DescriptorHeaps[i].RTVHeap.GetAddressOf()));
					device->GetDevice()->CreateDescriptorHeap(&SRVHeapDesc, IID_PPV_ARGS(s_Data->DescriptorHeaps[i].SRVHeap.GetAddressOf()));
					device->GetDevice()->CreateDescriptorHeap(&CBVHeapDesc, IID_PPV_ARGS(s_Data->DescriptorHeaps[i].CBVHeap.GetAddressOf()));
					device->GetDevice()->CreateDescriptorHeap(&UAVHeapDesc, IID_PPV_ARGS(s_Data->DescriptorHeaps[i].UAVHeap.GetAddressOf()));
					device->GetDevice()->CreateDescriptorHeap(&SamplerHeapDesc, 
						IID_PPV_ARGS(s_Data->DescriptorHeaps[i].SamplerHeap.GetAddressOf()));
					device->GetDevice()->CreateDescriptorHeap(&DSVHeapDesc, IID_PPV_ARGS(s_Data->DescriptorHeaps[i].DSVHeap.GetAddressOf()));
				}
			});

			/*
			m_Context = Heimdall::Renderer::GetContext().As<DirectX12Context>();
			Microsoft::WRL::ComPtr<ID3D12Device> device = m_Context->GetDevice()->GetDevice();

			m_FrameIndex = 0;
			m_viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(Application::Get().GetWindow().GetWidth()),
				static_cast<float>(Application::Get().GetWindow().GetHeight()));
			m_ScissorRect = CD3DX12_RECT(0, 0, static_cast<LONG>(Application::Get().GetWindow().GetWidth()),
				static_cast<LONG>(Application::Get().GetWindow().GetHeight()));
			m_RTVDescriptorSize = 0;

			m_RTVDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

			m_FrameIndex = m_Context->GetSwapChain()->GetSwapChain()->GetCurrentBackBufferIndex();

			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
			rtvHeapDesc.NumDescriptors = 2;
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			DXCALL(device->CreateDescriptorHeap(&rtvHeapDesc,
				IID_PPV_ARGS(&m_RenderTargetViewHeap)));
			m_RenderTargetViewHeap->SetName(L"Render Target View Heap");

			D3D12_DESCRIPTOR_HEAP_DESC SRVHeapDesc = {};
			SRVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			SRVHeapDesc.NumDescriptors = 1;
			SRVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			DXCALL(device->CreateDescriptorHeap(&SRVHeapDesc, IID_PPV_ARGS(&m_ShaderResourceViewHeap)));
			m_ShaderResourceViewHeap->SetName(L"Shader ResourceViewHeap");

			m_RTVDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RenderTargetViewHeap->GetCPUDescriptorHandleForHeapStart());

			for (uint32_t i = 0; i < 2; i++)
			{
				m_Context->GetSwapChain()->GetSwapChain()->GetBuffer(i, IID_PPV_ARGS(&m_RenderTargets[i]));
				device->CreateRenderTargetView(m_RenderTargets[i].Get(), nullptr, rtvHandle);
				rtvHandle.Offset(1, m_RTVDescriptorSize);
				std::wstring temp(L"Render Target #%s", i);
				m_RenderTargets[i]->SetName(temp.c_str());
			}

			DXCALL(device->CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_CommandAllocator.GetAddressOf())));
			m_CommandAllocator->SetName(L"Main Command Allocator");

			CD3DX12_ROOT_SIGNATURE_DESC rootsignature;
			rootsignature.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			Microsoft::WRL::ComPtr<ID3DBlob> signature;
			Microsoft::WRL::ComPtr<ID3DBlob> error;

			DXCALL(D3D12SerializeRootSignature(&rootsignature, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
			DXCALL(device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(),
				IID_PPV_ARGS(&m_RootSignature)));
			m_RootSignature->SetName(L"Main Root Signature");

			DXCALL(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
				m_CommandAllocator.Get(), m_PipelineState.Get(), IID_PPV_ARGS(&m_CommandList)));

			m_CommandList->SetName(L"Main Command List");

			DXCALL(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));
			m_FenceValue++;
			m_Fence->SetName(L"Main Fence");

			DXCALL(m_CommandList->Close());

			//device->Release();*/
		}

		void DirectX12Renderer::Shutdown()
		{

		}

		void DirectX12Renderer::BeginFrame()
		{
			/*m_FrameIndex = m_Context->GetSwapChain()->GetSwapChain()->GetCurrentBackBufferIndex();

			m_CommandAllocator->Reset();

			m_CommandList->Reset(m_CommandAllocator.Get(), m_PipelineState.Get());


			m_CommandList->SetDescriptorHeaps(1, m_ShaderResourceViewHeap.GetAddressOf());

			m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
			m_CommandList->RSSetViewports(1, &m_viewport);
			m_CommandList->RSSetScissorRects(1, &m_ScissorRect);

			m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_RenderTargets[m_FrameIndex].Get(),
				D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RenderTargetViewHeap->GetCPUDescriptorHandleForHeapStart(), m_FrameIndex, m_RTVDescriptorSize);
			m_CommandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

			const float clearcolor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
			m_CommandList->ClearRenderTargetView(rtvHandle, clearcolor, 0, nullptr);
			m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);*/


		}

		void DirectX12Renderer::EndFrame()
		{
			/*m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_RenderTargets[m_FrameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

			m_CommandList->Close();

			ID3D12CommandList* ppCommandList[] = { m_CommandList.Get() };
			m_Context->GetCurrentDevice()->GetGraphicsQueue()->ExecuteCommandLists(_countof(ppCommandList), ppCommandList);
			m_Context->GetSwapChain()->GetSwapChain()->Present(0, 0);
			WaitForFrame();*/
		}

		void DirectX12Renderer::WaitForFrame()
		{

			m_fenceEvent = CreateEvent(nullptr, false, false, nullptr);
			if (m_fenceEvent == nullptr)
			{
				HRESULT_FROM_WIN32(GetLastError());
			}

			const uint64_t fence = m_FenceValue;
			DXCALL(m_Context->GetCurrentDevice()->GetGraphicsQueue()->Signal(m_Fence.Get(), fence));+
			m_FenceValue++;

			if (m_Fence->GetCompletedValue() < fence)
			{
				DXCALL(m_Fence->SetEventOnCompletion(fence, m_fenceEvent));
				WaitForSingleObject(m_fenceEvent, INFINITE);
			}
		}
	}
}