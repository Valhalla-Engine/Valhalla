module;
#include <vector>
#include <string>
#include <unordered_map>


export module Valhalla.Renderer.Mesh;

import Valhalla.Core.Math.AABB;
import Valhalla.Math;

import Valhalla.Asset;

import Valhalla.Renderer.IndexBuffer;
import Valhalla.Renderer.UniformBuffer;
import Valhalla.Renderer.VertexBuffer;


export namespace Valhalla
{
	namespace Yggdrasil
	{
		class DirectX12Renderer;
		class VulkanRenderer;
	}

	namespace Mimir
	{
		class AssimpMeshImporter;
	}

	export namespace Heimdall
	{
		struct Vertex
		{
			Math::Float3 Position;
			Math::Float3 Normal;
			Math::Float3 Tangent;
			Math::Float3 BiNormal;
			Math::Float2 TexCoord;
		};

		const int numAttributes = 5;

		struct Index
		{
			uint32_t V1, V2, V3;
		};

		static_assert(sizeof(Index) == 3 * sizeof(uint32_t));

		struct Triangle
		{
			Vertex V0, V1, V2;

			Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
				: V0(v0), V1(v1), V2(v2) {}
		};

		class Submesh
		{
		public:
			uint32_t BaseVertex;
			uint32_t BaseIndex;
			uint32_t IndexCount;
			uint32_t VertexCount;

			Math::Mat4x4 transform;
			Math::Mat4x4 LocalTransform;
			AABB BoundingBox;

			std::string NodeName, MeshName;
		};

		class MeshNode
		{
		public:
			uint32_t Parent = 0xffffffff;
			std::vector<uint32_t> Children;
			std::vector<uint32_t> Submeshes;

			std::string Name;
			Math::Mat4x4 LocalTransform;

			inline bool IsRoot() const { return Parent == 0xffffffff; }
		};

		class MeshSource : public Mimir::Asset
		{
		public:
			MeshSource() = default;
			MeshSource(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const Math::Mat4x4& transform);
			MeshSource(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const std::vector<Submesh>& submeshes);
			virtual ~MeshSource();

			void DumpVertexBuffer();

			std::vector<Submesh>& GetSubmeshes() { return m_Submeshes; }
			const std::vector<Submesh>& GetSubmeshes() const { return m_Submeshes; }

			const std::string& GetFilePath() const { return m_FilePath; }

			const std::vector<Triangle> GetTriangleCache(uint32_t index) const { return m_TriangleCache.at(index); }

			Ref<VertexBuffer> GetVertexBuffer() { return m_VertexBuffer; }
			Ref<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }

			static Mimir::AssetType GetStaticType() { return Mimir::AssetType::MeshSource; }
			virtual Mimir::AssetType GetAssetType() const override { return GetStaticType(); }

			const AABB& GetBoundingBox() const { return m_BoundingBox; }

			const MeshNode& GetRootNode() const { return m_Nodes[0]; }
			const std::vector<MeshNode>& GetNodes() const { return m_Nodes; }

		private:
			std::vector<Submesh> m_Submeshes;

			Ref<VertexBuffer> m_VertexBuffer;
			Ref<IndexBuffer> m_IndexBuffer;

			std::vector<Vertex> m_Vertices;
			std::vector<Index> m_Indices;

			std::unordered_map<uint32_t, std::vector<Triangle>> m_TriangleCache;

			AABB m_BoundingBox;

			std::string m_FilePath;

			std::vector<MeshNode> m_Nodes;

			friend class Renderer;
			friend class Yggdrasil::DirectX12Renderer;
			friend class Yggdrasil::VulkanRenderer;
			friend class Mimir::AssimpMeshImporter;
		};

		class StaticMesh : public Mimir::Asset
		{
		public:
			explicit StaticMesh(Ref<MeshSource> meshSource);
			StaticMesh(Ref<MeshSource> meshSource, const std::vector<uint32_t>& submeshes);
			StaticMesh(const Ref<StaticMesh>& other);
			virtual ~StaticMesh();

			std::vector<uint32_t>& GetSubmeshes() { return m_Submeshes; }
			const std::vector<uint32_t>& GetSubmeshes() const { return m_Submeshes; }

			void SetSubmeshes(const std::vector<uint32_t>& submeshes);

			Ref<MeshSource> GetMeshSource() { return m_MeshSource; }
			Ref<MeshSource> GetMeshSource() const { return m_MeshSource; }
			void SetMeshAsset(Ref<MeshSource>& meshAsset) { m_MeshSource = meshAsset; }

			static Mimir::AssetType GetStaticType() { return Mimir::AssetType::StaticMesh; }
			virtual Mimir::AssetType GetAssetType() const override { return GetStaticType(); }
		private:
			Ref<MeshSource> m_MeshSource;
			std::vector<uint32_t> m_Submeshes;

			friend class Renderer;
			friend class DirectX12Renderer;
			friend class VulkanRenderer;
		};
	}
}