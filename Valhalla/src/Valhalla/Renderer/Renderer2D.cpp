#include "vhpch.h"
#include "Renderer2D.h"

#include "Pipeline.h"
#include "Shader.h"
#include "Renderer.h"
#include "IndexBuffer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Valhalla
{
	namespace Heimdal
	{
		struct VALHALLA_API QuadVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;
			float_t TexIndex;
			float_t TilingFactor;
		};

		struct VALHALLA_API LineVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
		};

		struct VALHALLA_API Renderer2DData
		{
			static const uint32_t MaxQuads = 20000;
			static const uint32_t MaxVertices = MaxQuads * 4;
			static const uint32_t MaxIndices = MaxQuads * 6;
			static const uint32_t MaxTextureSlots = 32;

			static const uint32_t MaxLines = 10000;
			static const uint32_t MaxLineVertices = MaxLines * 2;
			static const uint32_t MaxLineIndices = MaxLines * 6;

			Ref<Pipeline> QuadPipeline;
			Ref<VertexBuffer> QuadVertexBuffer;
			Ref<IndexBuffer> QuadIndexBuffer;

			Ref<Shader> TextureShader;
			Ref<Texture2D> WhiteTexture;

			uint32_t QuadIndexCount = 0;
			QuadVertex* QuadVertexBufferBase = nullptr;
			QuadVertex* QuadVertexBufferPtr = nullptr;

			std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
			uint32_t TextureSlotIndex = 1;

			glm::vec4 QuadVertexPositions[4];

			// Lines
			Ref<Pipeline> LinePipeline;
			Ref<VertexBuffer> LineVertexBuffer;
			Ref<IndexBuffer> LineIndexBuffer;

			Ref<Shader> LineShader;

			uint32_t LineIndexCount = 0;
			LineVertex* LineVertexBufferBase = nullptr;
			LineVertex* LineVertexBufferPtr = nullptr;

			glm::mat4 CameraViewProj;
			bool DepthTest = true;

			Renderer2D::Statistics stats;
		};

		static Renderer2DData s_Data;

		void Renderer2D::Init()
		{
			{
				PipelineSpecification pipelineSpecification;
				pipelineSpecification.Layout =
				{
					{ShaderDataType::Float3, "a_Position"},
					{ShaderDataType::Float4, "a_Color"},
					{ShaderDataType::Float2, "a_TexCoord"},
					{ShaderDataType::Float, "a_TexIndex"},
					{ShaderDataType::Float, "a_TilingFactor"}
				};
				s_Data.QuadPipeline = Pipeline::Create(pipelineSpecification);

				s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
				s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

				uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

				uint32_t offset = 0;

				for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
				{
					quadIndices[i + 0] = offset + 0;
					quadIndices[i + 1] = offset + 1;
					quadIndices[i + 2] = offset + 2;

					quadIndices[i + 3] = offset + 2;
					quadIndices[i + 4] = offset + 3;
					quadIndices[i + 5] = offset + 0;

					offset += 4;
				}

				s_Data.QuadIndexBuffer = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
				delete[] quadIndices;
			}

			s_Data.WhiteTexture = Texture2D::Create(TextureFormat::RGBA, 1, 1);
			uint32_t whiteTextureData = 0xffffffff;
			s_Data.WhiteTexture->Lock();
			s_Data.WhiteTexture->GetWriteableBuffer().Write(&whiteTextureData, sizeof(uint32_t));
			s_Data.WhiteTexture->Unlock();

			s_Data.TextureShader = Shader::Create("assets/shaders/Renderer2D.glsl");

			// Set all texture slots to 0
			s_Data.TextureSlots[0] = s_Data.WhiteTexture;

			s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };


		}
	}
}