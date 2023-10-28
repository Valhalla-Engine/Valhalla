#pragma once
#include "Valhalla/Core/Ref.h"

#include "Valhalla/Renderer/ShaderUniform.h"

namespace Valhalla
{
	namespace Heimdall
	{
		enum class ShaderUniformType
		{
			None = 0, Bool, Int, UInt, Float, Vec2, Vec3, Vec4, Mat4,
			IVec2, IVec3, IVec4
		};

		class ShaderUniform
		{
		public:
			ShaderUniform() = default;
			ShaderUniform(std::string name, ShaderUniformType type, uint32_t size, uint32_t offset);

			const std::string& GetName() const { return m_Name; }
			ShaderUniformType GetType() const { return m_Type; }
			uint32_t GetSize() const { return m_Size; }
			uint32_t GetOffset() const { return m_Offset; }

			static constexpr std::string_view UniformTypeToString(ShaderUniformType type);
		private:
			std::string m_Name;
			ShaderUniformType m_Type = ShaderUniformType::None;
			uint32_t m_Size = 0;
			uint32_t m_Offset = 0;
		};

		struct ShaderUniformBuffer
		{
			std::string Name;
			uint32_t Index;
			uint32_t BindingPoint;
			uint32_t Size;
			uint32_t RendererID;
			std::vector<ShaderUniform> Uniforms;
		};

		struct ShaderStorageBuffer
		{
			std::string Name;
			uint32_t Index;
			uint32_t Binding;
			uint32_t Size;
			uint32_t RendererID;
		};

		struct ShaderBuffer
		{
			std::string Name;
			uint32_t Size = 0;
			std::unordered_map<std::string, ShaderUniform> Uniforms;
		};

		class Shader : public RefCounted
		{
		public:
			using ShaderReloadedCallback = std::function<void()>;

			virtual void Reload(bool forceCompile = false) = 0;
			virtual void RT_Reload(bool forceCompile) = 0;

			virtual size_t GetHash() const = 0;

			virtual const std::string& GetName() const = 0;

			virtual void SetMacro(const std::string& name, const std::string& value) = 0;

			static Ref<Shader> Create(const std::string& filepath, bool forceCompile = false, bool disableOptimization = false);
			static Ref<Shader> CreateFromString(const std::string& source);

			virtual void AddShaderReloadedCallback(const ShaderReloadedCallback& callback) = 0;

			virtual const std::unordered_map<std::string, ShaderBuffer>& GetShaderBuffers() const = 0;
			virtual const std::unordered_map<std::string, ShaderResourceDeclaration>& GetResources() const = 0;

			static constexpr const char* GetShaderDirectoryPath()
			{
				return "Resources/Shaders/";
			}
		};

		class ShaderLibrary : public RefCounted
		{
		public:
			ShaderLibrary();
			~ShaderLibrary();

			void Add(const Ref<Shader>& shader);
			void Load(std::string_view path, bool forceCompile = false, bool disableOptimization = false);
			void Load(std::string_view name, const std::string& filepath);

			const Ref<Shader>& Get(const std::string& name) const;
			size_t GetSize() const { return m_Shaders.size(); }

			std::unordered_map<std::string, Ref<Shader>>& GetShaders() { return m_Shaders; }
			const std::unordered_map<std::string, Ref<Shader>>& GetShaders() const { return m_Shaders; }
		private:
			std::unordered_map<std::string, Ref<Shader>> m_Shaders;
		};
	}
}