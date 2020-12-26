#pragma once

#include "Valhalla/Core/Base.h"

#include <vector>
#include <glm/glm.hpp>

#include "Valhalla/Core/Timestep.h"

#include "Pipeline.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "Material.h"

#include "Valhalla/Core/Math/AABB.h"

struct aiNode;
struct aiAnimation;
struct aiNodeAnim;
struct aiScene;

namespace Assimp
{
	class Importer;
}

namespace Valhalla
{
	namespace Heimdal
	{
		struct VALHALLA_API Vertex
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec3 Tagent;
			glm::vec3 Binormal;
			glm::vec2 Texcoord;
		};

		struct VALHALLA_API AnimatedVertex
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec3 Tagent;
			glm::vec3 Binormal;
			glm::vec2 Texcoord;

			uint32_t IDs[4] = { 0, 0, 0, 0 };
			float_t Weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

			void AddBoneData(uint32_t BoneID, float_t Weight)
			{
				for (size_t i = 0; i < 4; i++)
				{
					if (Weights[i] == 0.0f)
					{
						IDs[i] = BoneID;
						Weights[i] = Weight;
						return;
					}
				}

				VH_CORE_WARN("Vertex has more than four bones/weights affecting it, extra data will be discarded (BoneID={0}, Weight={1})", BoneID, Weight);
			}
		};

		static const int NumAttributes = 5;

		struct VALHALLA_API Index
		{
			uint32_t V1, V2, V3;
		};

		static_assert(sizeof(Index) == 3 * sizeof(uint32_t));

		struct VALHALLA_API BoneInfo
		{
			glm::mat4 BoneOffset;
			glm::mat4 FinalTransformation;
		};

		struct VALHALLA_API VertexBoneData
		{
			uint32_t IDs[4];
			float_t Weights[4];

			VertexBoneData()
			{
				memset(IDs, 0, sizeof(IDs));
				memset(Weights, 0, sizeof(Weights));
			};

			void AddBoneData(uint32_t BoneID, float_t Weight)
			{
				for (size_t i = 0; i < 4; i++)
				{
					if (Weights[i] == 0.0f)
					{
						IDs[i] = BoneID;
						Weights[i] = Weight;
						return;
					}
				}

				VH_CORE_ASSERT(false, "Too many bones!");
			}
		};

		struct VALHALLA_API Triangle
		{
			Vertex V0, V1, V2;
			Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
				: V0(v0), V1(v1), V2(v2) {}
		};

		class VALHALLA_API Submesh
		{
		public:
			uint32_t BaseVertex;
			uint32_t BaseIndex;
			uint32_t MaterialIndex;
			uint32_t IndexCount;

			glm::mat4 Transform;
			AABB BoundingBox;

			std::string NodeName, MeshName;
		};

		class VALHALLA_API Mesh : public RefCounted
		{
		public:
			Mesh(const std::string& filename);
			~Mesh();

			void OnUpdate(Timestep ts);
			void DumpVertexBuffer();

			std::vector<Submesh>& GetSubmeshes() { return m_SubMeshes; }
			const std::vector<Submesh>& GetSubmeshes() const { return m_SubMeshes; }

			Ref<Shader> GetMeshShader() { return m_MeshShader; }
			Ref<Material> GetMaterial() { return m_BaseMaterial; }
			std::vector<Ref<MaterialInstance>> GetMaterials() { return m_Materials; }
			const std::vector<Ref<Texture2D>>& GetTextures() { return m_Textures; }
			const std::string& GetFilePath() const { return m_FilePath; }

			const std::vector<Triangle> GetTriangleCache(uint32_t index) const { return m_TriangleCache.at(index); }

			Ref<VertexBuffer> GetVertexBuffer() { return m_VertexBuffer; }
			Ref<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }
			const VertexBufferLayout& VertexBufferLayout() { return m_VertexBufferLayout; }

			void* GetDescriptionSet();
		private:
			void BoneTransform(float_t time);
			void ReadNodeHierarchy(float_t AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);
			void TraverseNodes(aiNode* node, const glm::mat4& parentTransform = glm::mat4(1.0f), uint32_t level = 0);

			const aiNodeAnim* FindNodeAnum(const aiAnimation* animation, const std::string& nodeName);
			uint32_t FindPosition(float_t AnimationTime, const aiNodeAnim* pNodeAnim);
			uint32_t FindRotation(float_t AnimationTime, const aiNodeAnim* pNodeAnim);
			uint32_t FindScaling(float_t AnimationTime, const aiNodeAnim* pNodeAnim);
			glm::vec3 InterpolateTranslation(float_t AnimatinoTime, const aiNodeAnim* nodeAnim);
			glm::quat InterpolateRotation(float_t AnimationTime, const aiNodeAnim* nodeAnim);
			glm::vec3 InterpolateScale(float_t AnimationTime, const aiNodeAnim* nodeAnim);
		private:
			std::vector<Submesh> m_SubMeshes;

			std::unique_ptr<Assimp::Importer> m_Importer;

			glm::mat4 m_InverseTransform;

			uint32_t m_BoneCount = 0;
			std::vector<BoneInfo> m_BoneInfo;

			Ref<Pipeline> m_Pipeline;
			Ref<VertexBuffer> m_VertexBuffer;
			Ref<IndexBuffer> m_IndexBuffer;
			VertexBufferLayout m_VertexBufferLayout;

			std::vector<Vertex> m_StaticVerrices;
			std::vector<AnimatedVertex> m_AnimatedVertices;
			std::vector<Index> m_Indices;
			std::unordered_map<std::string, uint32_t> m_BoneMapping;
			std::vector<glm::mat4> m_BoneTransform;
			const aiScene* m_Scene;

			// Materials
			Ref<Shader> m_MeshShader;
			Ref<Material> m_BaseMaterial;
			std::vector<Ref<Texture2D>> m_Textures;
			std::vector<Ref<Texture2D>> m_NormalMaps;
			std::vector<Ref<MaterialInstance>> m_Materials;

			std::unordered_map<uint32_t, std::vector<Triangle>> m_TriangleCache;

			// Animation
			bool m_IsAnimated = false;
			float_t m_AnimationTime = 0.0f;
			float_t m_WorldTime = 0.0f;
			float_t m_TimeMultiplier = 1.0f;
			bool m_AnimationPlaying = true;

			std::string m_FilePath;

			friend class Renderer;
			friend class SceneHierarchyPanel;
		};
	}
}
