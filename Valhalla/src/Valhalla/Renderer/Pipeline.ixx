export module Valhalla.Renderer.Pipeline;

import Valhalla.Core.Ref;

import Valhalla.Renderer.VertexBuffer;
import Valhalla.Renderer.Shader;
import Valhalla.Renderer.UniformBuffer;

export namespace Valhalla
{
	export namespace Heimdall
	{
		enum class PrimitiveTopology
		{
			None = 0,
			Points,
			Lines,
			Triangles,
			LineStrip,
			TriangleStrip,
			TriangleFan
		};

		enum class DepthCompareOperator
		{
			None = 0,
			Never,
			NotEqual,
			Less,
			LessOrEqual,
			Greater,
			GreaterOrEqual,
			Equal,
			Always
		};

		struct PipelineSpecification
		{
			Ref<Shader> Shader;
			VertexBufferLayout Layout;
			PrimitiveTopology Topology = PrimitiveTopology::Triangles;
			DepthCompareOperator DepthOperator = DepthCompareOperator::GreaterOrEqual;
			bool BackfaceCulling = true;
			bool DepthTest = true;
			bool DepthWrite = true;
			bool WireFrame = false;
			float LineWidth = 1.0f;

			std::string DebugName;
		};

		class Pipeline : public RefCounted
		{
		public:
			virtual ~Pipeline() = default;

			virtual PipelineSpecification& GetSpecification() = 0;
			virtual const PipelineSpecification& GetSpecification() const = 0;

			virtual void Invalidate() = 0;
			virtual void SetUniformBuffer(Ref<UniformBuffer> uniformBuffer, uint32_t binding, uint32_t set = 0) = 0;

			static Ref<Pipeline> Create(const PipelineSpecification& specs);
		};
	}
}