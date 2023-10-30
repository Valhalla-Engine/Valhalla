#include "vhpch.h"
#include "DirectX12Pipeline.h"
#include "DirectX12Shader.h"
#include "DirectX12Context.h"

#include "Valhalla/Renderer/Renderer.h"
//#include <d3dcommon.h>
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

				VH_CORE_ASSERT(false, "Unknown Topology");
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

				VH_CORE_ASSERT(false, "Unknown Depth Compare Operator");
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

			VH_CORE_ASSERT(false);
			DXGI_FORMAT_R32G32B32A32_TYPELESS;
		}

		DirectX12Pipeline::DirectX12Pipeline(const Heimdall::PipelineSpecification& specs)
			: m_Specification(specs)
		{
			VH_CORE_ASSERT(specs.shader);
			Invalidate();
			Heimdall::Renderer::RegisterShaderDependency(specs.shader, this);
		}

		void DirectX12Pipeline::Invalidate()
		{
			Ref<DirectX12Pipeline> instance = this;
			Heimdall::Renderer::Submit([instance]() mutable
			{
				auto device = DirectX12Context::GetCurrentDevice();
				Ref<DirectX12Shader> directX12Shader = Ref<DirectX12Shader>(instance->m_Specification.shader);

				D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc;
				pipelineDesc.pRootSignature = instance->m_PipelineLayout.Get();
				pipelineDesc.VS = directX12Shader->GetVertexShader();
				pipelineDesc.PS = directX12Shader->GetPixelShader();
				pipelineDesc.InputLayout = directX12Shader->GetInputLayout();
				pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
				pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
				pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
				pipelineDesc.SampleMask = UINT_MAX;
				pipelineDesc.PrimitiveTopologyType = Utils::GetDirectXTopology(instance->m_Specification.Topology);
				pipelineDesc.NumRenderTargets = 1;
				pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
				pipelineDesc.SampleDesc.Count = 0;
				pipelineDesc.SampleDesc.Quality = 1;
				pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

				DXCALL(device->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, 
					IID_PPV_ARGS(instance->m_Pipeline.GetAddressOf())));
			});
		}
	}
}