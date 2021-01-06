#pragma once

#include "Valhalla/Core/Ref.h"

#include "VertexBuffer.h"
#include "Shader.h"

namespace Valhalla::Heimdal
{
	struct PipelineSpecification
	{
		Ref<Shader> Shader;
		VertexBufferLayout Layout;
	};

	class VALHALLA_API Pipeline : public RefCounted
	{
	public:
		virtual ~Pipeline() = default;

		virtual PipelineSpecification& GetSpecification() = 0;
		virtual const PipelineSpecification& GetSpecification() const = 0;

		virtual void Invalidate() = 0;

		virtual void Bind() = 0;

		static Ref<Pipeline> Create(const PipelineSpecification& spec);
	};
}