#pragma once

#include "Valhalla/Core/Base.h"

#include "FrameBuffer.h"

namespace Valhalla::Heimdal
{
	struct RenderPassSpecification
	{
		Ref<FrameBuffer> TargetFrameBuffer;
	};

	class RenderPass : public RefCounted
	{
	public:
		virtual ~RenderPass() = default;

		virtual RenderPassSpecification& GetSpecification() = 0;
		virtual const RenderPassSpecification& GetSpecification() const = 0;

		static Ref<RenderPass> Create(const RenderPassSpecification& spec);
	};
}