#pragma once

#include "Valhalla/Renderer/RendererAPI.h"
#include "Valhalla/Renderer/Texture.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		class OpenGLTexture2D : public Heimdal::Texture2D
		{
		public:
			OpenGLTexture2D(Heimdal::TextureFormat format, uint32_t width, uint32_t height, Heimdal::TextureWrap wrap);
			OpenGLTexture2D(const std::string& path, bool srgb);
			virtual ~OpenGLTexture2D();

			virtual void Bind(uint32_t slot = 0) const override;

			virtual Heimdal::TextureFormat GetFormat() const override { return m_Format; }
			virtual uint32_t GetWidth() const override { return m_Width; }
			virtual uint32_t GetHeight() const override { return m_Height; }

			virtual uint32_t GetMipLevelCount() const override;

			virtual void Lock() override;
			virtual void Unlock() override;

			virtual void Resize(uint32_t width, uint32_t height) override;
			virtual Buffer GetWriteableBuffer() override;

			virtual const std::string& GetPath() const override { return m_FilePath; }

			virtual bool Loaded() const override { return m_Loaded; }

			virtual Heimdal::RendererID GetRendererID() const override { return m_RendererID; }

			virtual bool operator==(const Texture& other) const override
			{
				return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
			}

		private:
			Heimdal::RendererID m_RendererID = 0;
			Heimdal::TextureFormat m_Format = Heimdal::TextureFormat::None;
			Heimdal::TextureWrap m_Wrap = Heimdal::TextureWrap::Clamp;
			uint32_t m_Width, m_Height;

			Buffer m_ImageData;
			bool m_IsHDR = false;

			bool m_Locked = false;
			bool m_Loaded = false;

			std::string m_FilePath;
		};

		class OpenGLTextureCube : public Heimdal::TextureCube
		{
		public:
			OpenGLTextureCube(Heimdal::TextureFormat format, uint32_t width, uint32_t height);
			OpenGLTextureCube(const std::string& filepath);
			virtual ~OpenGLTextureCube();

			virtual void Bind(uint32_t slot = 0) const override;

			virtual Heimdal::TextureFormat GetFormat() const override { return m_Format; }
			virtual uint32_t GetWidth() const override { return m_Width; }
			virtual uint32_t GetHeight() const override { return m_Height; }
			virtual uint32_t GetMipLevelCount() const override;

			virtual const std::string& GetPath() const override { return m_FilePath; }

			virtual Heimdal::RendererID GetRendererID() const override { return m_RendererID; }

			virtual bool operator==(const Texture& other) const override
			{
				return m_RendererID == ((OpenGLTextureCube&)other).m_RendererID;
			}

		private:
			Heimdal::RendererID m_RendererID;
			Heimdal::TextureFormat m_Format;
			uint32_t m_Width, m_Height;

			uint8_t* m_ImageData;

			std::string m_FilePath;
		};
	}
}