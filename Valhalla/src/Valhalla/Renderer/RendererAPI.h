#pragma once

#include <memory>
#include <string>

namespace Valhalla
{
	namespace Heimdal
	{
		using RendererID = uint32_t;

		enum class RendererAPIType
		{
			None,
			OpenGL
		};

		enum class PrimitiveType
		{
			None = 0, Triangles, Lines
		};

		struct RenderAPICapabilities
		{
			std::string Vendor;
			std::string Renderer;
			std::string Version;

			int32_t MaxSamples = 0;
			float_t MaxAnisotropy = 0.0f;
			int32_t MaxTextureUnits = 0;
		};

		class RendererAPI : public RefCounted
		{
		public:
			static void Init();
			static void Shutdown();
			
			static void Clear(float_t r, float_t g, float_t b, float_t a);
			static void SetClearColor(float_t r, float_t g, float_t b, float_t a) ;
			
			static void DrawIndexed(uint32_t count, PrimitiveType type, bool depthTest = true);
			static void SetLineThickness(float_t thickness);

			static RenderAPICapabilities& GetCapabilities()
			{
				static RenderAPICapabilities capabilities;
				return capabilities;
			}

			static RendererAPIType Current() { return s_CurrentRendererAPI; }

		private:
			static void LoadRequiredAssets();

		private:
			static RendererAPIType s_CurrentRendererAPI;
		};
	}
}