#pragma once

#include "Valhalla/Core/Base.h"
#include "Valhalla/Core/Buffer.h"

#include "RendererAPI.h"
#include "ShaderUniform.h"

#include <string>
#include <glm/glm.hpp>

namespace Valhalla
{
	namespace Heimdal
	{
		enum class VALHALLA_API UniformType
		{
			None = 0,
			Float, Float2, Flaot3, Float4,
			Matrix3x3, Matrix4x4,
			Int32, Uint32
		};

		struct VALHALLA_API UniformDecl
		{
			UniformType Type;
			std::ptrdiff_t Offset;
			std::string Name;
		};

		struct VALHALLA_API UniformBuffer
		{
			byte* Buffer;
			std::vector<UniformDecl> uniforms;
		};

		struct VALHALLA_API UniformBufferBase
		{
			virtual const byte* GetBuffer() const = 0;
			virtual const UniformDecl* GetUniforms() const = 0;
			virtual uint32_t GetUniformCount() const = 0;
		};

		template<uint32_t N, uint32_t U>
		struct VALHALLA_API UniformBufferDeclaration : public UniformBufferBase
		{
			byte Buffer[N];
			UniformDecl Uniforms[U];
			std::ptrdiff_t Cursor = 0;
			int32_t Index = 0;

			virtual const byte* GetBuffer() const override { return Buffer; }
			virtual const UniformDecl* GetUniforms() const override { return Uniforms; }
			virtual uint32_t GetUniformCount() const override { return U; }

			template<typename T>
			void Push(const std::string& name, const T& data) {}

			template<>
			void Push(const std::string& name, const float_t& data)
			{
				Uniforms[Index++] = { UniformType::Float, Cursor, name };
				memcpy(Buffer + Cursor, &data, sizeof(float_t));
				Cursor += sizeof(float_t);
			}

			template<>
			void Push(const std::string& name, const glm::vec3& data)
			{
				Uniforms[Index++] = { UniformType::Float3, Cursor, name };
				memcpy(Buffer + Cursor, glm::value_ptr(data), sizeof(glm::vec3));
				Cursor += sizeof(glm::vec3);
			}

			template<>
			void Push(const std::string& name, const glm::vec4& data)
			{
				Uniforms[Index++] = { UniformType::Float4, Cursor, name };
				memcpy(Buffer + Cursor, glm::value_ptr(data), sizeof(glm::vec4));
				Cursor += sizeof(glm::vec4);
			}

			template<>
			void Push(const std::string& name, const glm::mat4& data)
			{
				Uniforms[Index++] = { UniformType::Matrix4x4, Cursor, name };
				memcpy(Buffer + Cursor, glm::value_ptr(data), sizeof(glm::mat4));
				Cursor += sizeof(glm::mat4);
			}

		};
		
		enum class VALHALLA_API ShaderUniformType
		{
			None = 0, Bool, Int, Float, Vec2, Vec3, Vec4, Mat3, Mat4
		};

		class VALHALLA_API ShaderUniform
		{
		public:
			ShaderUniform() = default;
			ShaderUniform(const std::string& name, ShaderUniformType type, uint32_t size, uint32_t offset);

			const std::string& GetName() const { return m_Name; }
			ShaderUniformType GetType() const { return m_Type; }
			uint32_t GetSize() const { return m_Size; }
			uint32_t GetOffset() const { return m_Offset; }

			static const std::string& UniformTypeToString(ShaderUniformType type);
		private:
			std::string m_Name;
			ShaderUniformType m_Type = ShaderUniformType::None;
			uint32_t m_Size = 0;
			uint32_t m_Offset = 0;
		};

		struct VALHALLA_API ShaderUniformBuffer
		{
			std::string Name;
			uint32_t Index;
			uint32_t BindingPoint;
			uint32_t Size;
			RendererID RendererID;
			std::vector<ShaderUniform> Uniforms;
		};

		struct VALHALLA_API ShaderBuffer
		{
			std::string Name;
			uint32_t Size = 0;
			std::unordered_map<std::string, ShaderUniform> Uniforms;
		};

		class VALHALLA_API Shader : public RefCounted
		{
		public:
			using ShaderReloadedCallback = std::function<void()>;

			virtual void Reloaded() = 0;

			virtual void Bind() = 0;
			virtual RendererID GetRendererID() = 0;

			// NEW shader system
			virtual void SetUniformBuffer(const std::string& name, const void* data, uint32_t size) = 0;
			virtual void SetUniform(const std::string& fullname, float_t value) = 0;
			virtual void SetUniform(const std::string& fullname, int32_t value) = 0;
			virtual void SetUniform(const std::string& fullname, const glm::vec2& value) = 0;
			virtual void SetUniform(const std::string& fullname, const glm::vec3& value) = 0;
			virtual void SetUniform(const std::string& fullname, const glm::vec4& value) = 0;
			virtual void SetUniform(const std::string& fullname, const glm::mat3& value) = 0;
			virtual void SetUniform(const std::string& fullname, const glm::mat4& value) = 0;

			// Temporary while there is no material system
			virtual void SetFloat(const std::string& name, float_t value) = 0;
			virtual void SetInt(const std::string& name, int32_t value) = 0;
			virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
			virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
			virtual void SetMat4FromRenderThread(const std::string& name, const glm::mat4& value, bool bind = true) = 0;

			virtual void SetIntArray(const std::string& name, int32_t* values, uint32_t size) = 0;

			virtual const std::string& GetName() const = 0;

			// Represent a complete shader program stored in a single file
			// Note: currently for simplicity this is simply a string filepath, however
			//		in the future this will be an asset object + metadata
			static Ref<Shader> Create(const std::string& filepath);
			static Ref<Shader> CreateFromString(const std::string& source);

			virtual const std::unordered_map<std::string, ShaderBuffer>& GetShaderBuffer() const = 0;
			virtual const std::unordered_map<std::string, ShaderResourceDeclaration>& GetResource() const = 0;

			virtual void AddShaderReloadedCallback(const ShaderReloadedCallback callback) = 0;

			// Temporary until there is an asset manager
			static std::vector<Ref<Shader>>s_AllShaders;
		};

		// This should eventually be handled by the asset manager
		class VALHALLA_API ShaderLibrary : public RefCounted
		{
			ShaderLibrary();
			~ShaderLibrary();

			void Add(const Ref<Shader>& shader);
			void Load(const std::string& path);
			void Load(const std::string& name, const std::string& path);

			const Ref<Shader>& Get(const std::string& name) const;

		private:
			std::unordered_map<std::string, Ref<Shader>> m_Shaders;
		};
	}
}