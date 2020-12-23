#pragma once

#include "Valhalla/Core/Base.h"

#include "Framebuffer.h"

namespace Valhalla
{
	namespace Heimdal
	{
		struct VALHALLA_API RenderPassSpecification
		{
			Ref<Framebuffer> TargetFramebuffer;
		};

		class VALHALLA_API RenderPass : public RefCounted
		{
		public:
			virtual ~RenderPass() = default;

			virtual RenderPassSpecification& GetSpecification() = 0;
			virtual const RenderPassSpecification& GetSpecification() const = 0;

			static Ref<RenderPass> Create(const RenderPassSpecification& spec);
		};

	}
}