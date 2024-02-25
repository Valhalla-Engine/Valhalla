#pragma once

#include "Valhalla/Core/Base.h"
#include "Valhalla/Renderer/Shader.h"
#include "Valhalla/Renderer/RenderCommandBuffer.h"

namespace Valhalla
{
	namespace Heimdall
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