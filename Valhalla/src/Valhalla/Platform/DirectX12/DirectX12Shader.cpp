#include "vhpch.h"

#include "DirectX12Shader.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		DirectX12Shader::DirectX12Shader(const std::string& path, bool forceCompile, bool disableOptimization)
		{
			DXCALL(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(m_Utils.GetAddressOf())));
			DXCALL(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(m_Compiler.GetAddressOf())));
			DXCALL(m_Utils->CreateDefaultIncludeHandler(m_IncludeHandler.GetAddressOf()));

			DXCALL(CompileShader(path, L"VSMain", L"vs_6_0", disableOptimization)->GetResult(m_VertexShader.GetAddressOf()));
			DXCALL(CompileShader(path, L"PSMain", L"ps_6_0", disableOptimization)->GetResult(m_PixelShader.GetAddressOf()));
		}

		DirectX12Shader::~DirectX12Shader()
		{
		}

		Microsoft::WRL::ComPtr<IDxcResult> DirectX12Shader::CompileShader(const std::string& path, LPCWSTR entrypoint, LPCWSTR target, bool disableOptimization)
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
			if (errors && errors->GetStringPointer() > 0)
			{
				VH_CORE_ERROR(errors->GetStringPointer());
			}
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
		}

		void DirectX12Shader::AddShaderReloadedCallback(const ShaderReloadedCallback& callback)
		{

		}

		D3D12_SHADER_BYTECODE DirectX12Shader::GetVertexShader()
		{
			D3D12_SHADER_BYTECODE bytecode = {};
			bytecode.pShaderBytecode = m_VertexShader->GetBufferPointer();
			bytecode.BytecodeLength = m_VertexShader->GetBufferSize();
			return bytecode;
		}

		D3D12_SHADER_BYTECODE DirectX12Shader::GetPixelShader()
		{
			D3D12_SHADER_BYTECODE bytecode = {};
			bytecode.pShaderBytecode = m_PixelShader->GetBufferPointer();
			bytecode.BytecodeLength = m_PixelShader->GetBufferSize();
			return bytecode;
		}
	}
}