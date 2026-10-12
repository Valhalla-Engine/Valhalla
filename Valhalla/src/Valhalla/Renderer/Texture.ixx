module;
#include <filesystem>

export module Valhalla.Renderer.Texture;

import Valhalla.Renderer.Image;
import Valhalla.Core.Buffer;
import Valhalla.Renderer.RendererResource;

import Valhalla.Math;


export namespace Valhalla
{
	export namespace Heimdall
	{
		struct TextureSpecification
		{
			ImageFormat Format = ImageFormat::RGBA;
			uint32_t Width = 1;
			uint32_t Height = 1;
			TextureWrap SamplerWrap = TextureWrap::Repeat;
			TextureFilter SamplerFilter = TextureFilter::Linear;

			bool GenerateMips = true;
			bool SRGB = false;
			bool Storage = false;
			bool StoreLocally = false;

			std::string DebugName;
		};

		class Texture : public RendererResource
		{
		public:
			virtual ~Texture() {}

			virtual void Bind(uint32_t slot = 0) const = 0;

			virtual ImageFormat GetFormat() const = 0;
			virtual uint32_t GetWidth() const = 0;
			virtual uint32_t GetHeight() const = 0;
			virtual Math::UInt2 GetSize() const = 0;

			virtual uint32_t GetMipLevelCount() const = 0;
			virtual std::pair<uint32_t, uint32_t> GetMipSize(uint32_t mip) const = 0;

			virtual uint64_t GetHash() const = 0;

			virtual TextureType GetType() const = 0;
		};


		class Texture2D : public Texture
		{
			static Ref<Texture2D> Create(const TextureSpecification& specification);
			static Ref<Texture2D> Create(const TextureSpecification& specification, const std::filesystem::path& filepath);
			static Ref<Texture2D> Create(const TextureSpecification& specification, Buffer imageData);

			virtual void Resize(const Math::UInt2& size) = 0;
			virtual void Resize(const uint32_t width, const uint32_t height) = 0;

			virtual Ref<Image2D> GetImage() const = 0;

			virtual void Lock() = 0;
			virtual void Unlock() = 0;

			virtual Buffer GetWriteableBuffer() = 0;

			virtual bool Loaded() const = 0;

			virtual const std::filesystem::path& GetPath() const = 0;

			virtual TextureType GetType() const override { return TextureType::Texture2D; }

			static Mimir::AssetType GetStaticType() { return Mimir::AssetType::Texture; }
			virtual Mimir::AssetType GetAssetType() const override { return GetStaticType(); }
		};

		class TextureCube : public Texture
		{
			static Ref<TextureCube> Create(const TextureSpecification& specification, Buffer imageData = Buffer());

			virtual TextureType GetType() const override { return TextureType::TextureCube; }

			static Mimir::AssetType GetStaticType() { return Mimir::AssetType::EnvMap; }
			virtual Mimir::AssetType GetAssetType() const override { return GetStaticType(); }
		};
	}
}