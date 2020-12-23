#pragma once

#include "Valhalla/Core/Base.h"

#include <string>

namespace Valhalla
{
	namespace Heimdal
	{
		using RendererID = uint32_t;

		enum class VALHALLA_API RendererAPIType
		{
			None,
			OpenGL,
			DirectX11,
			DirectX12,
			Vulkan,
			Metal
		};

		enum class VALHALLA_API PrimitiveType
		{
			None = 0, Triangles, Lines
		};

		struct VALHALLA_API RendererAPICapabilities
		{
			std::string Vendor;
			std::string Renderer;
			std::string Version;

			int32_t MaxSamples = 0;
			float_t MaxAnisotropy = 0.0f;
			int32_t MaxTextureUnits = 0;
		};

		class VALHALLA_API RendererAPI
		{
		public:
			static void Init();
			static void Shutdown();

			static void Clear(float_t r, float_t g, float_t b, float_t a);
			static void SetClearColor(float_t r, float_t g, float_t b, float_t a);

			static void DrawIndexed(uint32_t count, PrimitiveType type, bool depthTest = true);
			static void SetLineThickness(float_t thickness);

			static RendererAPICapabilities& GetCapabilities()
			{
				static RendererAPICapabilities capabilities;
				return capabilities;
			}

			static RendererAPIType Current() { return s_CurrentRendererAPI; }
			static void SetAPI(RendererAPI api);

		private:
			static void LoadRequiredAssets();

		private:
			static RendererAPIType s_CurrentRendererAPI;
		};
	}
}