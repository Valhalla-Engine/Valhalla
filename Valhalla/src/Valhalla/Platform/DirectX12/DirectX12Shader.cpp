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
		}

		DirectX12Shader::~DirectX12Shader()
		{
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
	}
}