
export module Valhalla.Renderer.ComputePass;

import Valhalla.Renderer.FrameBuffer;
import Valhalla.Renderer.UniformBufferSet;
import Valhalla.Renderer.UniformBuffer;
import Valhalla.Renderer.StorageBufferSet;
import Valhalla.Renderer.StorageBuffer;
import Valhalla.Renderer.Texture;
import Valhalla.Renderer.Image;
import Valhalla.Renderer.PipelineCompute;
import Valhalla.Math;

import Valhalla.Core.Ref;

import <string>;


export namespace Valhalla
{
	export namespace Heimdall
	{
		struct ComputePassSpecification
		{
			Ref<PipelineCompute> Pipeline;
			std::string DebugName;
			Math::Float4 MarkerColor;
		};

		class ComputePass : public RefCounted
		{
		public:
			virtual ~ComputePass() = default;

			virtual ComputePassSpecification& GetSpecification() = 0;
			virtual const ComputePassSpecification& GetSpecification() const = 0;

			virtual void SetInput(std::string_view name, Ref<UniformBufferSet> uniformBufferSet) = 0;
			virtual void SetInput(std::string_view name, Ref<UniformBuffer> uniformBuffer) = 0;

			virtual void SetInput(std::string_view name, Ref<StorageBufferSet> uniformBufferSet) = 0;
			virtual void SetInput(std::string_view name, Ref<StorageBuffer> storageBuffer) = 0;

			virtual void SetInput(std::string_view name, Ref<Texture2D> texture) = 0;
			virtual void SetInput(std::string_view name, Ref<TextureCube> textureCube) = 0;
			virtual void SetInput(std::string_view name, Ref<Image2D> image) = 0;

			virtual Ref<Image2D> GetOutput(uint32_t index) = 0;
			virtual Ref<Image2D> GetDepthOutput() = 0;
			virtual bool HasDescriptorSets() const = 0;
			virtual uint32_t GetFirstSetIndex() const = 0;

			virtual bool Validate() = 0;
			virtual void Bake() = 0;
			virtual bool Baked() = 0;
			virtual void Prepare() = 0;

			virtual Ref<PipelineCompute> GetPipeline() const = 0;

			static Ref<ComputePass> Create(const ComputePassSpecification& spec);
		};
	}
}