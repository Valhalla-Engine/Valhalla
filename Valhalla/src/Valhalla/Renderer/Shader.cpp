#include "vhpch.h"
#include "Shader.h"


#include "Valhalla/Platform/DirectX12/DirectX12Shader.h"

#include "Valhalla/Renderer/RendererAPI.h"

namespace Valhalla
{
	namespace Heimdall
	{
		Ref<Shader> Shader::Create(const std::string& filepath, bool forceCompile, bool disableOptimization)
		{
			Ref<Shader> result = nullptr;

			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:
				{
					break;
				}

				case RendererAPIType::DirectX12:
				{
					result = Ref<Yggdrasil::DirectX12Shader>::Create(filepath, forceCompile, disableOptimization);
					break;
				}

				case RendererAPIType::Vulkan:
				{
					break;
				}
			}

			return result;
		}

		Ref<Shader> Shader::CreateFromString(const std::string& source)
		{
			Ref<Shader> result = nullptr;

			switch (RendererAPI::Current())
			{
				case RendererAPIType::None:		return nullptr;
			}

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

		void ShaderLibrary::Load(std::string_view path, bool forceCompile, bool disableOptimization)
		{
			Ref<Shader> shader = Shader::Create(path.data(), forceCompile, disableOptimization);
			VH_CORE_ASSERT(m_Shaders.find(shader->GetName()) == m_Shaders.end());
			std::string name = shader->GetName();
			m_Shaders[shader->GetName()] = shader;
		}

		void ShaderLibrary::Load(std::string_view name, const std::string& path)
		{
			VH_CORE_ASSERT(m_Shaders.find(std::string(name)) == m_Shaders.end());
			m_Shaders[std::string(name)] = Shader::Create(path);
		}

		const Ref<Shader>& ShaderLibrary::Get(const std::string& name) const
		{
			VH_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end());
			return m_Shaders.at(name);
		}

		ShaderUniform::ShaderUniform(std::string name, const ShaderUniformType type, const uint32_t size, const uint32_t offset)
			: m_Name(std::move(name)), m_Type(type), m_Size(size), m_Offset(offset)
		{}

		constexpr std::string_view ShaderUniform::UniformTypeToString(const ShaderUniformType type)
		{
			if (type == ShaderUniformType::Bool)
			{
				return std::string("Boolean");
			}
			else if (type == ShaderUniformType::Int)
			{
				return std::string("Int");
			}
			else if (type == ShaderUniformType::Float)
			{
				return std::string("Float");
			}

			return std::string("None");
		}
	}
}