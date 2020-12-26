#include "vhpch.h"
#include "Shader.h"

namespace Valhalla
{
	namespace Heimdal
	{
		std::vector<Ref<Shader>> Shader::s_AllShaders;

		Ref<Shader> Shader::Create(const std::string& filepath)
		{
			Ref<Shader> result = nullptr;

			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::OpenGL:		return nullptr;
				case RendererAPIType::DirectX11:	VH_CORE_ASSERT(false, "DirectX 11 is currently not supported");  return nullptr;
				case RendererAPIType::DirectX12:	VH_CORE_ASSERT(false, "DirectX 12 is currently not supported"); return nullptr;
				case RendererAPIType::Vulkan:		VH_CORE_ASSERT(false, "Vulkan is currently not supported"); return nullptr;
				case RendererAPIType::Metal:		VH_CORE_ASSERT(false, "Metal is currently not supported"); return nullptr;
			}

			s_AllShaders.push_back(result);
			return result;
		}

		Ref<Shader> Shader::CreateFromString(const std::string& source)
		{
			Ref<Shader> result = nullptr;

			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:			return nullptr;
				case RendererAPIType::OpenGL:		return nullptr;
				case RendererAPIType::DirectX11:	VH_CORE_ASSERT(false, "DirectX 11 is currently not supported");  return nullptr;
				case RendererAPIType::DirectX12:	VH_CORE_ASSERT(false, "DirectX 12 is currently not supported"); return nullptr;
				case RendererAPIType::Vulkan:		VH_CORE_ASSERT(false, "Vulkan is currently not supported"); return nullptr;
				case RendererAPIType::Metal:		VH_CORE_ASSERT(false, "Metal is currently not supported"); return nullptr;
			}

			s_AllShaders.push_back(result);
			return result;
		}

		ShaderLibrary::ShaderLibrary()
		{
		}

		ShaderLibrary::~ShaderLibrary()
		{
		}

		void ShaderLibrary::Add(const Ref<Shader>& shader)
		{
			auto& name = shader->GetName();
			VH_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end());
			m_Shaders[name] = shader;
		}

		void ShaderLibrary::Load(const std::string& path)
		{
			auto shader = Ref<Shader>(Shader::Create(path));
			auto& name = shader->GetName();
			VH_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end());
			m_Shaders[name] = shader;
		}

		void ShaderLibrary::Load(const std::string& name, const std::string& path)
		{
			VH_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end());
			m_Shaders[name] = Ref<Shader>(Shader::Create(path));
		}

		const Ref<Shader>& ShaderLibrary::Get(const std::string& name) const
		{
			VH_CORE_ASSERT(m_Shaders.find(name) != m_Shaders.end());
			return m_Shaders.at(name);
		}

		ShaderUniform::ShaderUniform(const std::string& name, ShaderUniformType type, uint32_t size, uint32_t offset)
			: m_Name(name), m_Type(type), m_Size(size), m_Offset(offset) {}

		const std::string& ShaderUniform::UniformTypeToString(ShaderUniformType type)
		{
			switch (type)
			{
				case ShaderUniformType::None:	return "None";
				case ShaderUniformType::Bool:	return "Boolean";
				case ShaderUniformType::Int:	return "Int";
				case ShaderUniformType::Float:	return "Float";
				case ShaderUniformType::Vec2:	return "2D Vector";
				case ShaderUniformType::Vec3:	return "3D Vector";
				case ShaderUniformType::Vec4:	return "4D Vector";
				case ShaderUniformType::Mat3:	return "3x3 Matrix";
				case ShaderUniformType::Mat4:	return "4x4 Matrix";
			}

			return "None";
		}
	}
}