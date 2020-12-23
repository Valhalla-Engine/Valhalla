#pragma once

#include "Valhalla/Core/Base.h"

#include "VertexBuffer.h"
#include "Shader.h"
#include "RenderPass.h"

namespace Valhalla
{
	namespace Heimdal
	{
		struct VALHALLA_API PipelineSpecification
		{
			Ref<Shader> Shader;
			VertexBufferLayout Layout;
			Ref<RenderPass> RenderPass;
		};

		class VALHALLA_API Pipeline : public RefCounted
		{
		public:
			virtual ~Pipeline() = default;

			virtual PipelineSpecification& GetSpecification() = 0;
			virtual const PipelineSpecification& GetSpecification() const = 0;

			virtual void Invalidate() = 0;

			// TEMP: remove this when render command buffers are a thing
			virtual void Bind() = 0;

			static Ref<Pipeline> Create(const PipelineSpecification& spec);
		};
	}
}