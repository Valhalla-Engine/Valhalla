#include "vhpch.h"
#include "OpenGLPipeline.h"

#include "Valhalla/Renderer/Renderer.h"

#include <glad/glad.h>

namespace Valhalla::Yggdrasil
{
	static GLenum ShaderDataTypeToOpenGLBaseType(Heimdal::ShaderDataType type)
	{
		switch (type)
		{
			case Heimdal::ShaderDataType::Float:	return GL_FLOAT;
			case Heimdal::ShaderDataType::Float2:	return GL_FLOAT;
			case Heimdal::ShaderDataType::Float3:	return GL_FLOAT;
			case Heimdal::ShaderDataType::Float4:	return GL_FLOAT;
			case Heimdal::ShaderDataType::Mat3:		return GL_FLOAT;
			case Heimdal::ShaderDataType::Mat4:		return GL_FLOAT;
			case Heimdal::ShaderDataType::Int:		return GL_INT;
			case Heimdal::ShaderDataType::Int2:		return GL_INT;
			case Heimdal::ShaderDataType::Int3:		return GL_INT;
			case Heimdal::ShaderDataType::Int4:		return GL_INT;
			case Heimdal::ShaderDataType::Bool:		return GL_BOOL;
		}

		VH_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	OpenGLPipeline::OpenGLPipeline(const Heimdal::PipelineSpecification& spec)
		: m_Specification(spec) 
	{
		Invalidate();
	}

	OpenGLPipeline::~OpenGLPipeline()
	{
		GLuint rendererID = m_VertexArrayRendererID;
		Heimdal::Renderer::Submit([rendererID]()
			{
				glDeleteVertexArrays(1, &rendererID);
			});
	}

	void OpenGLPipeline::Invalidate()
	{
		VH_CORE_ASSERT(m_Specification.Layout.GetElements().size(), "Layout is empty");

		Ref<OpenGLPipeline>instance = this;
		Heimdal::Renderer::Submit([instance]() mutable
		{
				auto& vertexArrayRendererID = instance->m_VertexArrayRendererID;

				if (vertexArrayRendererID)
					glDeleteVertexArrays(1, &vertexArrayRendererID);

				glGenVertexArrays(1, &vertexArrayRendererID);
				glBindVertexArray(vertexArrayRendererID);

				glBindVertexArray(0);
		});
	}

	void OpenGLPipeline::Bind()
	{
		Ref<OpenGLPipeline> instance = this;
		Heimdal::Renderer::Submit([instance]() 
		{
				glBindVertexArray(instance->m_VertexArrayRendererID);

				const auto& layout = instance->m_Specification.Layout;
				uint32_t attribIndex = 0;
				for (const auto& element : layout)
				{
					auto glBaseType = ShaderDataTypeToOpenGLBaseType(element.Type);
					glEnableVertexAttribArray(attribIndex);
					if (glBaseType == GL_INT)
					{
						glVertexAttribIPointer(attribIndex,
							element.GetComponentCount(),
							glBaseType,
							layout.GetStride(),
							(const void*)(uintptr_t)element.Offset);
					}
					else
					{
						glVertexAttribPointer(attribIndex,
							element.GetComponentCount(),
							glBaseType,
							element.Normalized ? GL_TRUE : GL_FALSE,
							layout.GetStride(),
							(const void*)(intptr_t)element.Offset);
					}
					attribIndex++;
				}
		});
	}
}