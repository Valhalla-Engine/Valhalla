#include "vhpch.h"
#include "Shader.h"

//#include "Renderer.h"

namespace Valhalla::Heimdal
{
	std::vector<Ref<Shader>> Shader::s_AllShaders;

	Ref<Shader> Shader::Create(const std::string& filename)
	{
		Ref<Shader> result = nullptr;

		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:		return nullptr;
			case RendererAPIType::OpenGL:	result = nullptr;
		}
		s_AllShaders.push_back(result);
		return nullptr;
	}

	Ref<Shader> Shader::CreateFromString(const std::string& source)
	{
		Ref<Shader> result = nullptr;

		switch (RendererAPI::Current())
		{
			case RendererAPIType::None:		return nullptr;
			case RendererAPIType::OpenGL:	result = nullptr;
		}
		s_AllShaders.push_back(result);
		return nullptr;
	}

	ShaderLibrary::ShaderLibrary()
	{

	}

	ShaderLibrary::~ShaderLibrary()
	{

	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		const std::string& name = shader->GetName();
		VH_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end());
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Load(const std::string& filename)
	{
		Ref<Shader> shader = Ref<Shader>(Shader::Create(filename));
		const std::string& name = shader->GetName();
		VH_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end());
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Load(const std::string& name, const std::string& filename)
	{
		VH_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end());
		m_Shaders[name] = Ref<Shader>(Shader::Create(filename));
	}

	const Ref<Shader>& ShaderLibrary::Get(const std::string& name) const
	{
		VH_CORE_ASSERT(m_Shaders.find(name) == m_Shaders.end());
		return m_Shaders.at(name);
	}
}