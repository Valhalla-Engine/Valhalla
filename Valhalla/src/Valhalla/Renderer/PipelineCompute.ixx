export module Valhalla.Renderer.PipelineCompute;

import Valhalla.Core.Ref;
import Valhalla.Renderer.Shader;
import Valhalla.Renderer.RenderCommandBuffer;

export namespace Valhalla
{
	export namespace Heimdall
	{
		class PipelineCompute : public RefCounted
		{
		public:
			virtual void Begin(Ref<RenderCommandBuffer> renderCommandBuffer = nullptr) = 0;
			virtual void RT_Begin(Ref<RenderCommandBuffer> renderCommandBuffer = nullptr) = 0;
			virtual void End() = 0;

			virtual Ref<Shader> GetShader() const = 0;

			static Ref<PipelineCompute> Create(Ref<Shader> computeShader);
		};
	}
}