#include "D12Include.hpp"

module Valhalla.Yggdrasil.DirectX12.Pipeline;

import Valhalla.Core.Assert;

import Valhalla.Renderer.Renderer;

import Valhalla.Yggdrasil.DirectX12.Context;
import Valhalla.Yggdrasil.DirectX12.Shader;

import Valhalla.Core.Log;
import Valhalla.Yggdrasil.DirectX12.VD3D12;
#include "DXCALL.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		namespace Utils
		{
			static D3D12_PRIMITIVE_TOPOLOGY_TYPE GetDirectXTopology(Heimdall::PrimitiveTopology topology)
			{
				switch (topology)
				{
					case Heimdall::PrimitiveTopology::Points:			return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
					case Heimdall::PrimitiveTopology::Lines:			return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
					case Heimdall::PrimitiveTopology::Triangles:		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
					case Heimdall::PrimitiveTopology::LineStrip:		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
					case Heimdall::PrimitiveTopology::TriangleStrip:	return D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
					case Heimdall::PrimitiveTopology::TriangleFan:		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
				}

				Assert(false, "Unknown Topology");
				return D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
			}

			static D3D12_COMPARISON_FUNC GetDirectXCompareOperator(const Heimdall::DepthCompareOperator compareOp)
			{
				switch (compareOp)
				{
					case Heimdall::DepthCompareOperator::Never:				return D3D12_COMPARISON_FUNC_NEVER;
					case Heimdall::DepthCompareOperator::NotEqual:			return D3D12_COMPARISON_FUNC_NOT_EQUAL;
					case Heimdall::DepthCompareOperator::Less:				return D3D12_COMPARISON_FUNC_LESS;
					case Heimdall::DepthCompareOperator::LessOrEqual:		return D3D12_COMPARISON_FUNC_LESS_EQUAL;
					case Heimdall::DepthCompareOperator::Greater:			return D3D12_COMPARISON_FUNC_GREATER;
					case Heimdall::DepthCompareOperator::GreaterOrEqual:	return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
					case Heimdall::DepthCompareOperator::Equal:				return D3D12_COMPARISON_FUNC_EQUAL;
					case Heimdall::DepthCompareOperator::Always:			return D3D12_COMPARISON_FUNC_ALWAYS;
				}

				Assert(false, "Unknown Depth Compare Operator");
				return D3D12_COMPARISON_FUNC_ALWAYS;
			}
		}

		static DXGI_FORMAT ShaderDataTypeToDirectXFormat(Heimdall::ShaderDataType type)
		{
			switch (type)
			{
				case Heimdall::ShaderDataType::Float:		return DXGI_FORMAT_R32_FLOAT;
				case Heimdall::ShaderDataType::Float2:		return DXGI_FORMAT_R32G32_FLOAT;
				case Heimdall::ShaderDataType::Float3:		return DXGI_FORMAT_R32G32B32_FLOAT;
				case Heimdall::ShaderDataType::Float4:		return DXGI_FORMAT_R32G32B32A32_FLOAT;
				case Heimdall::ShaderDataType::Int:			return DXGI_FORMAT_R32_SINT;
				case Heimdall::ShaderDataType::Int2:		return DXGI_FORMAT_R32G32_SINT;
				case Heimdall::ShaderDataType::Int3:		return DXGI_FORMAT_R32G32B32_SINT;
				case Heimdall::ShaderDataType::Int4:		return DXGI_FORMAT_R32G32B32A32_SINT;
			}

			Assert(false);
			DXGI_FORMAT_R32G32B32A32_TYPELESS;
		}

		DirectX12Pipeline::DirectX12Pipeline(const Heimdall::PipelineSpecification& specs)
			: m_Specification(specs)
		{
			Assert(specs.Shader);
			Invalidate();
			Heimdall::Renderer::RegisterShaderDependency(specs.Shader, this);
		}

		DirectX12Pipeline::~DirectX12Pipeline()
		{

		}

		void DirectX12Pipeline::Invalidate()
		{
			Ref<DirectX12Pipeline> instance = this;
			Heimdall::Renderer::Submit([instance]() mutable
			{
				auto device = DirectX12Context::GetCurrentDevice();
				Ref<DirectX12Shader> directX12Shader = instance->m_Specification.Shader.As<DirectX12Shader>();

				directX12Shader->GetVertexShader();
				auto inputLayout = directX12Shader->GetInputLayout();

				D3D12_INPUT_ELEMENT_DESC elementDesc = {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0};
				D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc;
				ZeroMemory(&pipelineDesc, sizeof(pipelineDesc));
				pipelineDesc.pRootSignature = directX12Shader->GetRootSignature().Get();
				pipelineDesc.VS = directX12Shader->GetVertexShader();
				pipelineDesc.PS = directX12Shader->GetPixelShader();
				pipelineDesc.InputLayout = { &elementDesc, 1};
				pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
				pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
				pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
				pipelineDesc.SampleMask = UINT_MAX;
				pipelineDesc.PrimitiveTopologyType = Utils::GetDirectXTopology(instance->m_Specification.Topology);
				pipelineDesc.NumRenderTargets = 1;
				pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
				pipelineDesc.SampleDesc.Count = 1;
				pipelineDesc.SampleDesc.Quality = 0;
				pipelineDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;



				DXCALL(device->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&instance->m_Pipeline)));
			});
		}

		void DirectX12Pipeline::SetUniformBuffer(Ref<Heimdall::UniformBuffer> uniformBuffer, uint32_t binding, uint32_t set /* = 0 */)
		{

		}

		void DirectX12Pipeline::RT_SetUniformBuffer(Ref<Heimdall::UniformBuffer> uniformBuffer, uint32_t binding, uint32_t set /* = 0 */)
		{

		}
	}
}