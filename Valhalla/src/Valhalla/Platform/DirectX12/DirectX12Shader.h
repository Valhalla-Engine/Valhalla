#pragma once

#include "VD3D12.h"
#include <dxcapi.h>
#include <d3d12shader.h>

#include "Valhalla/Renderer/Shader.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		class DirectX12Shader : public Heimdall::Shader
		{
		public:
			DirectX12Shader() = default;
			DirectX12Shader(const std::string& path, bool forceCompile, bool disableOptimization);
			virtual ~DirectX12Shader();
			void Release();

			void Reload(bool forceCompile = false) override;
			void RT_Reload(bool forceCompile) override;

			virtual size_t GetHash() const override;
			void SetMacro(const std::string& name, const std::string& value) override {}

			D3D12_INPUT_LAYOUT_DESC GetInputLayout();

			D3D12_SHADER_BYTECODE GetVertexShader() { return { m_VertexShader->GetBufferPointer(), m_VertexShader->GetBufferSize() }; }
			D3D12_SHADER_BYTECODE GetPixelShader() { return {m_PixelShader->GetBufferPointer(), m_PixelShader->GetBufferSize()}; }

			virtual const std::string& GetName() const override { return m_Name; }
			virtual const std::unordered_map<std::string, Heimdall::ShaderBuffer>& GetShaderBuffers() const override {  }
			virtual const std::unordered_map<std::string, Heimdall::ShaderResourceDeclaration>& GetResources() const override {}
			virtual void AddShaderReloadedCallback(const ShaderReloadedCallback& callback) override;

		private:

			Microsoft::WRL::ComPtr<IDxcResult> CompileShader(const std::string& path, LPCWSTR entrypoint, LPCWSTR target, bool disableOptimization);

		private:
			std::string m_Name;
			Microsoft::WRL::ComPtr<IDxcBlob> m_VertexShader;
			Microsoft::WRL::ComPtr<IDxcBlob> m_PixelShader;

			/*Compiler stuff*/
			Microsoft::WRL::ComPtr<IDxcCompiler3> m_Compiler;
			Microsoft::WRL::ComPtr<IDxcUtils> m_Utils;
			Microsoft::WRL::ComPtr<ID3D12ShaderReflection> m_Reflection;
			Microsoft::WRL::ComPtr<IDxcIncludeHandler> m_IncludeHandler;
		};
	}
}