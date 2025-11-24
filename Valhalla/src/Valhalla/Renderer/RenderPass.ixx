#include <DirectXMath.h>

export module Valhalla.Renderer.RenderPass;

import Valhalla.Renderer.FrameBuffer;
import Valhalla.Renderer.UniformBufferSet;
import Valhalla.Renderer.UniformBuffer;
import Valhalla.Renderer.StorageBufferSet;
import Valhalla.Renderer.StorageBuffer;
import Valhalla.Renderer.Texture;
import Valhalla.Renderer.Pipeline;
import Valhalla.Renderer.Image;

import Valhalla.Core.Ref;

import <string>;

export namespace Valhalla
{
	export namespace Heimdall
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