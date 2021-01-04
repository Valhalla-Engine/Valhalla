#pragma once

#include "Valhalla/Renderer/ShaderUniform.h"

namespace Valhalla::Yggdrasil
{
	class OpenGLShaderResourceDeclaration : public Heimdal::ShaderResourceDeclaration
	{
	public:
		enum class Type
		{
			NONE, TEXTURE2D, TEXTURECUBE
		};

	public:
		OpenGLShaderResourceDeclaration(Type type, const std::string& name, uint32_t count);

		inline const std::string& GetName() const override { return m_Name; }
		inline uint32_t GetRegister() const override { return m_Register; }
		inline uint32_t GetCount() const override { return m_Count; }

		inline Type GetType() const { return m_Type; }

		static Type StringToType(const std::string& type);
		static std::string TypeToString(Type type);

	private:
		friend class OpenGLShader;
	private:
		std::string m_Name;
		uint32_t m_Register = 0;
		uint32_t m_Count;
		Type m_Type;
	};

	class OpenGLShaderUniformDeclaration : public Heimdal::ShaderUniformDeclaration
	{
	public:
		enum class Type
		{
			NONE, FLOAT32, VEC2, VEC3, VEC4, MAT3, MAT4, INT32, BOOL, STRUCT
		};

		OpenGLShaderUniformDeclaration(Heimdal::ShaderDomain domain, Type type, const std::string& name, uint32_t count = 1);
		OpenGLShaderUniformDeclaration(Heimdal::ShaderDomain domain, Heimdal::ShaderStruct* uniformStruct, const std::string& name, uint32_t count = 1);

		inline const std::string& GetName() const override { return m_Name; }
		inline uint32_t GetSize() const override { return m_Size; }
		inline uint32_t GetCount() const override { return m_Count; }
		inline uint32_t GetOffset() const override { return m_Offset; }
		inline uint32_t GetAbsouluteOffset() const { return m_Struct ? m_Struct->GetOffset() + m_Offset : m_Offset; }
		inline Heimdal::ShaderDomain GetDomain() const override { return m_Domain; }

		int32_t GetLocation() const { return m_Location; }
		inline Type GetType() const { return m_Type; }
		inline bool IsArray() const { m_Count > 1; }
		inline const Heimdal::ShaderStruct& GetShaderUniformStruct() const { VH_CORE_ASSERT(m_Struct, "There is something wrong with m_Struct in OpenGL"); return *m_Struct; }

		static uint32_t SizeOfUniformType(Type type);
		static Type StringToType(const std::string& name);
		static std::string TypeToString(Type type);

	protected:
		void SetOffset(uint32_t offset) override;

	private:
		friend class OpenGLShader;
		friend class OpenGLShaderUniformBufferDeclaration;
	private:
		std::string m_Name;
		uint32_t m_Size;
		uint32_t m_Count;
		uint32_t m_Offset;
		Heimdal::ShaderDomain m_Domain;

		Type m_Type;
		Heimdal::ShaderStruct* m_Struct;
		mutable int32_t m_Location;
	};

	struct GLShaderUniformField
	{
		OpenGLShaderUniformDeclaration::Type type;
		std::string name;
		uint32_t count;
		mutable uint32_t size;
		mutable int32_t location;
	};

	class OpenGLShaderUniformBufferDeclaration : public Heimdal::ShaderUniformBufferDeclaration
	{
	public:
		OpenGLShaderUniformBufferDeclaration(const std::string& name, Heimdal::ShaderDomain domain);

		void PushUniform(OpenGLShaderUniformDeclaration* uniform);

		inline const std::string& GetName() const override { return m_Name; }
		inline uint32_t GetRegister() const override { return m_Register; }
		inline uint32_t GetSize() const override { return m_Size; }
		virtual Heimdal::ShaderDomain GetDomain() const { return m_Domain; }

		virtual const Heimdal::ShaderUniformList& GetUniformDeclarations() const override { return m_Uniforms; }

		Heimdal::ShaderUniformDeclaration* FindUniform(const std::string& name);
	private:
		friend class Shader;
	private:
		std::string m_Name;
		Heimdal::ShaderUniformList m_Uniforms;
		uint32_t m_Register;
		uint32_t m_Size;
		Heimdal::ShaderDomain m_Domain;
	};
}