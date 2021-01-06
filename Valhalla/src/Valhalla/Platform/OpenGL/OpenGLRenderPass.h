#pragma once

#include "Valhalla/Renderer/RenderPass.h"

namespace Valhalla::Yggdrasil
{
	class OpenGLRenderPass : public Heimdal::RenderPass
	{
	public:
		OpenGLRenderPass(const Heimdal::RenderPassSpecification& spec);
		virtual ~OpenGLRenderPass();

		virtual Heimdal::RenderPassSpecification& GetSpecification() override { return m_Specification; }
		virtual const Heimdal::RenderPassSpecification& GetSpecification() const override { return m_Specification; }

	private:
		Heimdal::RenderPassSpecification m_Specification;
	};
}