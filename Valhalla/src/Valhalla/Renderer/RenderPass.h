#pragma once

#include "Valhalla/Core/Base.h"

#include "Valhalla/Renderer/FrameBuffer.h"
#include "Valhalla/Renderer/UniformBufferSet.h"
#include "Valhalla/Renderer/StorageBufferSet.h"
#include "Valhalla/Renderer/Texture.h"
#include "Valhalla/Renderer/Pipeline.h"

namespace Valhalla
{
	namespace Heimdall
	{
		struct RenderPassSpecification
		{
			Ref<Pipeline> Pipeline;
			std::string DebugName;
			DirectX::XMFLOAT4 MarkerColor;
		};

		class RenderPass : public RefCounted
		{
		public:
			virtual ~RenderPass() = default;

			virtual RenderPassSpecification& GetSpecification() = 0;
			virtual const RenderPassSpecification& GetSpecification() const = 0;

			virtual void SetInput(std::string_view name, Ref<UniformBufferSet> uniformBufferSet) = 0;
			virtual void SetInput(std::string_view name, Ref<UniformBuffer> uniformBuffer) = 0;

			virtual void SetInput(std::string_view name, Ref<StorageBufferSet> storageBufferSet) = 0;
			virtual void SetInput(std::string_view name, Ref<StorageBuffer> storageBuffer) = 0;

			virtual void SetInput(std::string_view name, Ref<Texture2D> texture) = 0;
			virtual void SetInput(std::string_view name, Ref<TextureCube> textureCube) = 0;
			virtual void SetInput(std::string_view name, Ref<Image2D> image) = 0;

			virtual Ref<Image2D> GetOutput(uint32_t index) = 0;
			virtual Ref<Image2D> GetDepthOutput() = 0;
			virtual uint32_t GetFirstSetIndex() const = 0;

			virtual Ref<Pipeline> GetPipeline() const = 0;
			virtual Ref<FrameBuffer> GetTargetFrameBuffer() const = 0;

			virtual bool Validate() = 0;
			virtual void Bake() = 0;
			virtual bool Baked() = 0;
			virtual void Prepate() = 0;

			static Ref<RenderPass> Create(const RenderPassSpecification& spec);
		};
	}
}