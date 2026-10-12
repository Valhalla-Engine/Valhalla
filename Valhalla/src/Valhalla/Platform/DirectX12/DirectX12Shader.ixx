module;
#include "D12Include.hpp"
#include <dxcapi.h>
#include <d3d12shader.h>
#include <d3dcompiler.h>
#include <unordered_map>
#include <string>

export module Valhalla.Yggdrasil.DirectX12.Shader;

import Valhalla.Renderer.Shader;



export namespace Valhalla
{
	export namespace Yggdrasil
	{
		class DirectX12Shader : public Heimdall::Shader
		{
		public:
			struct ReflectionData
			{
				std::unordered_map<std::string, Heimdall::ShaderResourceDeclaration> Resources;
				std::unordered_map<std::string, Heimdall::ShaderBuffer> ConstantBuffers;
			};

			DirectX12Shader() = default;
			DirectX12Shader(const std::string& path, bool forceCompile, bool disableOptimization);
			virtual ~DirectX12Shader();
			void Release();

			void Reload(bool forceCompile = false) override;
			void RT_Reload(bool forceCompile) override;

			virtual size_t GetHash() const override;
			void SetMacro(const std::string& name, const std::string& value) override {}

			D3D12_INPUT_LAYOUT_DESC GetInputLayout() { return m_InputLayout;}
			void CreateRootSignature();

			D3D12_SHADER_BYTECODE GetVertexShader() { return { m_VertexShader->GetBufferPointer(), m_VertexShader->GetBufferSize() }; }
			D3D12_SHADER_BYTECODE GetPixelShader() { return {m_PixelShader->GetBufferPointer(), m_PixelShader->GetBufferSize()}; }

			Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature() { return m_RootSignature; }

			virtual const std::string& GetName() const override { return m_Name; }
			virtual const std::unordered_map<std::string, Heimdall::ShaderBuffer>& GetShaderBuffers() const override { return  m_ReflectionData.ConstantBuffers; }
			virtual const std::unordered_map<std::string, Heimdall::ShaderResourceDeclaration>& GetResources() const override { return m_ReflectionData.Resources; }
			virtual void AddShaderReloadedCallback(const ShaderReloadedCallback& callback) override;

		private:
			void CreateInputLayout();
			Microsoft::WRL::ComPtr<ID3D10Blob> CompileShader(const std::string& path, LPCSTR entrypoint, LPCSTR target, bool disableOptimization);

		private:
			std::string m_Name;
			
			Microsoft::WRL::ComPtr<ID3D10Blob> m_VertexShader;
			Microsoft::WRL::ComPtr<ID3D10Blob> m_PixelShader;
			Microsoft::WRL::ComPtr<ID3D12ShaderReflection> m_VertexReflection;
			Microsoft::WRL::ComPtr<ID3D12ShaderReflection> m_PixelReflection;
			Microsoft::WRL::ComPtr<ID3D12RootSignature> m_RootSignature;

			D3D12_INPUT_LAYOUT_DESC m_InputLayout;

			ReflectionData m_ReflectionData;

		};
	}
}