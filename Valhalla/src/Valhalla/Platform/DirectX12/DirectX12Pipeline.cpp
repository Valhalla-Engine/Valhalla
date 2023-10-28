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
			static D3D_PRIMITIVE_TOPOLOGY GetDirectXTopology(Heimdall::PrimitiveTopology topology)
			{
				switch (topology)
				{
					case Heimdall::PrimitiveTopology::Points:			return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
					case Heimdall::PrimitiveTopology::Lines:			return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
					case Heimdall::PrimitiveTopology::Triangles:		return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
					case Heimdall::PrimitiveTopology::LineStrip:		return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
					case Heimdall::PrimitiveTopology::TriangleStrip:	return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
					case Heimdall::PrimitiveTopology::TriangleFan:		return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
				}

				VH_CORE_ASSERT(false, "Unknown Topology");
				return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
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

				
			});
		}
	}
}