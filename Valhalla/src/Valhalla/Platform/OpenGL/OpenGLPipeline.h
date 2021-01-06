#pragma once

#include "Valhalla/Renderer/Pipeline.h"

namespace Valhalla::Yggdrasil
{
	class OpenGLPipeline : public Heimdal::Pipeline
	{
	public:
		OpenGLPipeline(const Heimdal::PipelineSpecification& spec);
		virtual ~OpenGLPipeline();

		virtual Heimdal::PipelineSpecification& GetSpecification() override { return m_Specification; }
		virtual const Heimdal::PipelineSpecification& GetSpecification() const override { return m_Specification; }

		virtual void Invalidate() override;

		virtual void Bind() override;

	private:
		Heimdal::PipelineSpecification m_Specification;
		uint32_t m_VertexArrayRendererID = 0;
	};
}