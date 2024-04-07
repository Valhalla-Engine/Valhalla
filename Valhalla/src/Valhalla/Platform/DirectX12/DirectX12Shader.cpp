#include "vhpch.h"

#include "DirectX12Shader.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		DirectX12Shader::DirectX12Shader(const std::string& path, bool forceCompile, bool disableOptimization)
		{
			m_Name = "Test";

			DXCALL(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(m_Utils.GetAddressOf())));
			DXCALL(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(m_Compiler.GetAddressOf())));
			DXCALL(m_Utils->CreateDefaultIncludeHandler(m_IncludeHandler.GetAddressOf()));

			auto vertexCompiled = CompileShader(path, L"VSMain", L"vs_6_0", disableOptimization);
			auto pixelCompiled = CompileShader(path, L"PSMain", L"ps_6_0", disableOptimization);

			DXCALL(vertexCompiled->GetResult(m_VertexShader.GetAddressOf()));
			DXCALL(pixelCompiled->GetResult(m_PixelShader.GetAddressOf()));

			Microsoft::WRL::ComPtr<IDxcBlob> reflectionblob;
			DXCALL(vertexCompiled->GetOutput(DXC_OUT_REFLECTION, IID_PPV_ARGS(reflectionblob.GetAddressOf()), nullptr));

			DxcBuffer reflectionBuffer;
			reflectionBuffer.Ptr = reflectionblob->GetBufferPointer();
			reflectionBuffer.Size = reflectionblob->GetBufferSize();
			reflectionBuffer.Encoding = 0;

			DXCALL(m_Utils->CreateReflection(&reflectionBuffer, IID_PPV_ARGS(m_Reflection.GetAddressOf())));
		}

		DirectX12Shader::~DirectX12Shader()
		{
		}

		Microsoft::WRL::ComPtr<IDxcResult> DirectX12Shader::CompileShader(const std::string& path, LPCWSTR entrypoint, 
			LPCWSTR target, bool disableOptimization)
		{
			std::vector<LPCWSTR> compilationArgs
			{
				L"-E",
				entrypoint,
				L"-T",
				target,
				DXC_ARG_PACK_MATRIX_ROW_MAJOR,
				DXC_ARG_WARNINGS_ARE_ERRORS,
				DXC_ARG_ALL_RESOURCES_BOUND
			};

			if constexpr (VH_DEBUG)
			{
				compilationArgs.push_back(DXC_ARG_DEBUG);
			}
			else if (disableOptimization)
			{
				compilationArgs.push_back(DXC_ARG_OPTIMIZATION_LEVEL0);
			}
			else
			{
				compilationArgs.push_back(DXC_ARG_OPTIMIZATION_LEVEL3);
			}

			Microsoft::WRL::ComPtr<IDxcBlobEncoding> source;
			std::wstring temp = std::wstring(path.begin(), path.end());
			DXCALL(m_Utils->LoadFile(temp.c_str(), nullptr, source.GetAddressOf()));

			DxcBuffer sourceBuffer;
			sourceBuffer.Ptr = source->GetBufferPointer();
			sourceBuffer.Size = source->GetBufferSize();
			sourceBuffer.Encoding = 0;

			Microsoft::WRL::ComPtr<IDxcResult> result;
			DXCALL(m_Compiler->Compile(&sourceBuffer, compilationArgs.data(), static_cast<uint32_t>(compilationArgs.size()), m_IncludeHandler.Get(),
				IID_PPV_ARGS(&result)));

			Microsoft::WRL::ComPtr<IDxcBlobUtf8> errors;
			DXCALL(result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr));
			if (errors && errors->GetStringPointer() && errors->GetStringPointer()[0] != '\0')
			{
				VH_CORE_ERROR(errors->GetStringPointer());
			}

			return result;
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

		D3D12_INPUT_LAYOUT_DESC DirectX12Shader::GetInputLayout()
		{
			D3D12_SHADER_DESC shaderDesc{};
			DXCALL(m_Reflection->GetDesc(&shaderDesc));

			std::vector<D3D12_INPUT_ELEMENT_DESC> InputElements;
			for (size_t i = 0; i < shaderDesc.InputParameters; i++)
			{
				D3D12_SIGNATURE_PARAMETER_DESC signatureDesc;
				m_Reflection->GetInputParameterDesc(i, &signatureDesc);

				D3D12_INPUT_ELEMENT_DESC desc;
				desc.SemanticName = signatureDesc.SemanticName;
				desc.SemanticIndex = signatureDesc.SemanticIndex;
				desc.InputSlot = 0;
				desc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
				desc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
				desc.InstanceDataStepRate = 0;

				if (signatureDesc.Mask == 1)
				{
					switch (signatureDesc.ComponentType)
					{
						case D3D_REGISTER_COMPONENT_UINT32:		desc.Format = DXGI_FORMAT_R32_UINT; break;
						case D3D_REGISTER_COMPONENT_SINT32:		desc.Format = DXGI_FORMAT_R32_SINT; break;
						case D3D_REGISTER_COMPONENT_FLOAT32:	desc.Format = DXGI_FORMAT_R32_FLOAT; break;
					}
				}
				else if (signatureDesc.Mask <= 3)
				{
					switch (signatureDesc.ComponentType)
					{
						case D3D_REGISTER_COMPONENT_UINT32:		desc.Format = DXGI_FORMAT_R32G32_UINT; break;
						case D3D_REGISTER_COMPONENT_SINT32:		desc.Format = DXGI_FORMAT_R32G32_SINT; break;
						case D3D_REGISTER_COMPONENT_FLOAT32:	desc.Format = DXGI_FORMAT_R32G32_FLOAT; break;
					}
				}
				else if (signatureDesc.Mask <= 7)
				{
					switch (signatureDesc.ComponentType)
					{
						case D3D_REGISTER_COMPONENT_UINT32:		desc.Format = DXGI_FORMAT_R32G32B32_UINT; break;
						case D3D_REGISTER_COMPONENT_SINT32:		desc.Format = DXGI_FORMAT_R32G32B32_SINT; break;
						case D3D_REGISTER_COMPONENT_FLOAT32:	desc.Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
					}
				}
				else if (signatureDesc.Mask <= 15)
				{
					switch (signatureDesc.ComponentType)
					{
						case D3D_REGISTER_COMPONENT_UINT32:		desc.Format = DXGI_FORMAT_R32G32B32A32_UINT; break;
						case D3D_REGISTER_COMPONENT_SINT32:		desc.Format = DXGI_FORMAT_R32G32B32A32_SINT; break;
						case D3D_REGISTER_COMPONENT_FLOAT32:	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
					}
				}

				InputElements.push_back(desc);
			}

			return { InputElements.data(), static_cast<uint32_t>(InputElements.size()) };
		}

		std::vector<D3D12_ROOT_PARAMETER1> DirectX12Shader::GetRootParameters()
		{
			std::vector<D3D12_ROOT_PARAMETER1> rootParameters;

			D3D12_SHADER_DESC shaderDesc{};
			DXCALL(m_Reflection->GetDesc(&shaderDesc));

			for (size_t i = 0; i < shaderDesc.BoundResources; i++)
			{
				D3D12_SHADER_INPUT_BIND_DESC shaderBindDesc;
				DXCALL(m_Reflection->GetResourceBindingDesc(i, &shaderBindDesc));

				if (shaderBindDesc.Type = D3D_SIT_CBUFFER)
				{
					ID3D12ShaderReflectionConstantBuffer* shaderBuffer = m_Reflection->GetConstantBufferByIndex(i);

					D3D12_SHADER_BUFFER_DESC bufferDesc;
					shaderBuffer->GetDesc(&bufferDesc);

					D3D12_ROOT_PARAMETER1 root;
					root.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
					root.Descriptor = { shaderBindDesc.BindPoint, shaderBindDesc.Space, D3D12_ROOT_DESCRIPTOR_FLAG_NONE };

					rootParameters.push_back(root);
					shaderBuffer = nullptr;
				}

				else if (shaderBindDesc.Type == D3D_SIT_TEXTURE)
				{

				}
			}

			return rootParameters;
		}
	}
}