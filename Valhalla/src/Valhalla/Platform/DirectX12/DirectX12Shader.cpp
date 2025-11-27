#include "D12Include.hpp"

#include <cwchar>

#include <dxcapi.h>
#include <d3d12shader.h>
#include <d3dcompiler.h>


module Valhalla.Yggdrasil.DirectX12.Shader;

import Valhalla.Yggdrasil.DirectX12.Context;

import Valhalla.Core.Log;
import Valhalla.Yggdrasil.DirectX12.VD3D12;
#include "DXCALL.h"


namespace Valhalla
{
	namespace Yggdrasil
	{
		DirectX12Shader::DirectX12Shader(const std::string& path, bool forceCompile, bool disableOptimization)
		{
			m_VertexShader = CompileShader(path, "VSMain", "vs_5_0", disableOptimization);
			m_PixelShader = CompileShader(path, "PSMain", "ps_5_0", disableOptimization);

			DXCALL(D3DReflect(m_VertexShader->GetBufferPointer(), m_VertexShader->GetBufferSize(), IID_PPV_ARGS(&m_VertexReflection)));
			DXCALL(D3DReflect(m_PixelShader->GetBufferPointer(), m_PixelShader->GetBufferSize(), IID_PPV_ARGS(&m_PixelReflection)));

			CD3DX12_ROOT_SIGNATURE_DESC rootDesc;
			rootDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			Microsoft::WRL::ComPtr<ID3D10Blob> signature;
			Microsoft::WRL::ComPtr<ID3D10Blob> error;
			auto device = DirectX12Context::GetCurrentDevice();
			DXCALL(D3D12SerializeRootSignature(&rootDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
			DXCALL(device->GetDevice()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(m_RootSignature.GetAddressOf())));

			CreateInputLayout();
			//CreateRootSignature();
		}

		DirectX12Shader::~DirectX12Shader()
		{
		}

		Microsoft::WRL::ComPtr<ID3D10Blob> DirectX12Shader::CompileShader(const std::string& path, LPCSTR entrypoint, 
			LPCSTR target, bool disableOptimization)
		{
			Microsoft::WRL::ComPtr<ID3D10Blob> bytecode;
			Microsoft::WRL::ComPtr<ID3D10Blob> error;

			uint32_t compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef VH_DEBUG
			compileFlags |= D3DCOMPILE_DEBUG;
#endif
			if (disableOptimization)
				compileFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;

			std::size_t len = std::mbstowcs(nullptr, path.c_str(), 0);
			std::vector<wchar_t> buffer(len + 1);
			std::mbstowcs(buffer.data(), path.c_str(), len + 1);
			std::wstring wpath(buffer.data());

			DXCALL(D3DCompileFromFile(wpath.c_str(), nullptr, nullptr, entrypoint, target, compileFlags, 0, bytecode.GetAddressOf(), error.GetAddressOf()));

			if (error != nullptr)
				Log::CoreInfo(((char*)error->GetBufferPointer()));

			return bytecode;
		}


		void DirectX12Shader::Release()
		{
		}

		void DirectX12Shader::Reload(bool forceCompile)
		{
		}

		void DirectX12Shader::RT_Reload(bool forceCompile)
		{
		}

		size_t DirectX12Shader::GetHash() const
		{
			return 0;
		}

		void DirectX12Shader::AddShaderReloadedCallback(const ShaderReloadedCallback& callback)
		{

		}

		void DirectX12Shader::CreateInputLayout()
		{
			D3D12_SHADER_DESC shaderDesc{};
			DXCALL(m_VertexReflection->GetDesc(&shaderDesc));

			std::vector<D3D12_INPUT_ELEMENT_DESC> InputElements;
			uint32_t offset = 0;
			for (size_t i = 0; i < shaderDesc.InputParameters; i++)
			{
				D3D12_SIGNATURE_PARAMETER_DESC signatureDesc;
				m_VertexReflection->GetInputParameterDesc(i, &signatureDesc);

				D3D12_INPUT_ELEMENT_DESC desc;
				desc.SemanticName = signatureDesc.SemanticName;
				desc.SemanticIndex = signatureDesc.SemanticIndex;
				desc.InputSlot = 0;
				desc.AlignedByteOffset = offset;
				desc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
				desc.InstanceDataStepRate = 0;

				std::string temp(desc.SemanticName);
				
				if (temp == "POSITION")
				{
					desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
					offset += 12;
				}
				else if (signatureDesc.Mask == 1)
				{
					switch (signatureDesc.ComponentType)
					{
					case D3D_REGISTER_COMPONENT_UINT32:		desc.Format = DXGI_FORMAT_R32_UINT; offset+= 4; break;
					case D3D_REGISTER_COMPONENT_SINT32:		desc.Format = DXGI_FORMAT_R32_SINT; offset += 4; break;
					case D3D_REGISTER_COMPONENT_FLOAT32:	desc.Format = DXGI_FORMAT_R32_FLOAT; offset += 4; break;
					}
				}
				else if (signatureDesc.Mask <= 3)
				{
					switch (signatureDesc.ComponentType)
					{
					case D3D_REGISTER_COMPONENT_UINT32:		desc.Format = DXGI_FORMAT_R32G32_UINT; offset += 8; break;
					case D3D_REGISTER_COMPONENT_SINT32:		desc.Format = DXGI_FORMAT_R32G32_SINT; offset += 8; break;
					case D3D_REGISTER_COMPONENT_FLOAT32:	desc.Format = DXGI_FORMAT_R32G32_FLOAT; offset += 8; break;
					}
				}
				else if (signatureDesc.Mask <= 7)
				{
					switch (signatureDesc.ComponentType)
					{
					case D3D_REGISTER_COMPONENT_UINT32:		desc.Format = DXGI_FORMAT_R32G32B32_UINT; offset += 12; break;
					case D3D_REGISTER_COMPONENT_SINT32:		desc.Format = DXGI_FORMAT_R32G32B32_SINT; offset += 12; break;
					case D3D_REGISTER_COMPONENT_FLOAT32:	desc.Format = DXGI_FORMAT_R32G32B32_FLOAT; offset += 12; break;
					}
				}
				else if (signatureDesc.Mask <= 15)
				{
					switch (signatureDesc.ComponentType)
					{
					case D3D_REGISTER_COMPONENT_UINT32:		desc.Format = DXGI_FORMAT_R32G32B32A32_UINT; offset += 16; break;
					case D3D_REGISTER_COMPONENT_SINT32:		desc.Format = DXGI_FORMAT_R32G32B32A32_SINT; offset += 16; break;
					case D3D_REGISTER_COMPONENT_FLOAT32:	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; offset += 16; break;
					}
				}

				InputElements.push_back(desc);
			}
			D3D12_INPUT_ELEMENT_DESC* inputElementDescs = new D3D12_INPUT_ELEMENT_DESC[InputElements.size()];
			memcpy(inputElementDescs, InputElements.data(), InputElements.size() * sizeof(D3D12_INPUT_ELEMENT_DESC));

			D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
			m_InputLayout.pInputElementDescs = inputElementDescs;
			m_InputLayout.NumElements = static_cast<UINT>(InputElements.size());
		}

		void DirectX12Shader::CreateRootSignature()
		{
			D3D12_SHADER_DESC vertexShaderDesc;
			D3D12_SHADER_DESC pixelShaderDesc;
			DXCALL(m_VertexReflection->GetDesc(&vertexShaderDesc));
			DXCALL(m_PixelReflection->GetDesc(&pixelShaderDesc));

			uint32_t number = vertexShaderDesc.BoundResources;
			uint32_t constant = vertexShaderDesc.ConstantBuffers;

			uint32_t pixelnumber = pixelShaderDesc.BoundResources;
			uint32_t pixelConstant = pixelShaderDesc.ConstantBuffers;

			if (!(number && constant && pixelnumber && pixelConstant))
			{
				CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
				rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
				Microsoft::WRL::ComPtr<ID3D10Blob> signature;
				DXCALL(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr));
				DXCALL(DirectX12Context::Get()->GetCurrentDevice()->GetDevice()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)));
				return;
			}

			for (uint32_t i = 0; i < vertexShaderDesc.ConstantBuffers; i++)
			{
				D3D12_SHADER_BUFFER_DESC desc;
				auto buffer = m_VertexReflection->GetConstantBufferByIndex(i);
				buffer->GetDesc(&desc);
				std::string name(desc.Name);
			}

			for (uint32_t i = 0; i < vertexShaderDesc.BoundResources; i++)
			{
				D3D12_SHADER_INPUT_BIND_DESC desc;
				m_VertexReflection->GetResourceBindingDesc(i, &desc);
				std::string name(desc.Name);
			}

			Log::CoreInfo("Bound Resources: {0}", number);
			Log::CoreInfo("Constant buffers: {0}", constant);

			return ;
		}
	}
}
