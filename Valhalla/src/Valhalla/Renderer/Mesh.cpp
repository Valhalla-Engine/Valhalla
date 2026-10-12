module;
#include "imgui/imgui.h"
#include <vector>
#include <string>

module Valhalla.Renderer.Mesh;

import Valhalla.Core.Ref;



namespace Valhalla
{


/*#define MESH_DEBUG_LOG 0
#if MESH_DEBUG_LOG
#define VH_MESH_LOG(...) VH_CORE_TRACE_TAG("Mesh", __VA_ARGS__)
#define VH_MESH_ERROR(...) VH_CORE_ERROR_TAG("Mesh", __VA_ARGS__)
#else
#define VH_MESH_LOG(...)
#define VH_MESH_ERROR(...)
#endif*/

	namespace Heimdall
	{
		MeshSource::MeshSource(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const Math::Mat4x4& transform)
			: m_Vertices(vertices), m_Indices(indices)
		{
			Handle = {};

			Submesh submesh;
			submesh.BaseVertex = 0;
			submesh.BaseIndex = 0;
			submesh.IndexCount = (uint32_t)indices.size() * 3u;
			submesh.transform = transform;
			m_Submeshes.push_back(submesh);

			m_VertexBuffer = VertexBuffer::Create(m_Vertices.data(), static_cast<uint32_t>(m_Vertices.size() * sizeof(Vertex)));
			m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), static_cast<uint32_t>(m_Indices.size() * sizeof(Index)));
		}

		MeshSource::MeshSource(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const std::vector<Submesh>& submeshes)
			: m_Vertices(vertices), m_Indices(indices), m_Submeshes(submeshes)
		{
			Handle = {};

			m_VertexBuffer = VertexBuffer::Create(m_Vertices.data(), static_cast<uint32_t>(m_Vertices.size() * sizeof(Vertex)));
			m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), static_cast<uint32_t>(m_Indices.size() * sizeof(Index)));
		}

		MeshSource::~MeshSource()
		{

		}

		static std::string LevelToSpaces(uint32_t level)
		{
			std::string result = "";
			for (uint32_t i = 0; i < level; i++)
				result += "--";
			return result;
		}

		void MeshSource::DumpVertexBuffer()
		{
			//VH_MESH_LOG("----------------------------------------------------------");
			//VH_MESH_LOG("Vertex Buffer Dump");
			//VH_MESH_LOG("Mesh: {0}", m_FilePath);
			for (size_t i = 0; i < m_Vertices.size(); i++)
			{
				auto& vertex = m_Vertices[i];
				//VH_MESH_LOG("Vertex {0}", i);
				//VH_MESH_LOG("Position: {0}, {1}, {2}", vertex.Position.x, vertex.Position.y, vertex.Position.z);
				//VH_MESH_LOG("Normal: {0}, {1}, {2}", vertex.Normal.x, vertex.Normal.y, vertex.Normal.z);
				//VH_MESH_LOG("Binormal: {0}, {1}, {2}", vertex.BiNormal.x, vertex.BiNormal.y, vertex.BiNormal.z);
				//VH_MESH_LOG("Tangent: {0}, {1}, {2}", vertex.Tangent.x, vertex.Tangent.y, vertex.Tangent.z);
				//VH_MESH_LOG("TexCoord: {0}, {1}, {2}", vertex.TexCoord.x, vertex.TexCoord.y);
				//VH_MESH_LOG("--");
			}

			//VH_MESH_LOG("----------------------------------------------------------");
		}

		StaticMesh::StaticMesh(Ref<MeshSource> meshSource)
			: m_MeshSource(meshSource)
		{
			Handle = {};

			SetSubmeshes({});
		}

		StaticMesh::StaticMesh(Ref<MeshSource> meshSource, const std::vector<uint32_t>& submeshes)
			: m_MeshSource(meshSource)
		{
			Handle = {};

			SetSubmeshes(submeshes);
		}

		StaticMesh::StaticMesh(const Ref<StaticMesh>& other)
			: m_MeshSource(other->m_MeshSource)
		{
			SetSubmeshes(other->m_Submeshes);
		}

		StaticMesh::~StaticMesh()
		{
		}

		void StaticMesh::SetSubmeshes(const std::vector<uint32_t>& submeshes)
		{
			if (!submeshes.empty())
			{
				m_Submeshes = submeshes;
			}
			else
			{
				const auto& submeshes = m_MeshSource->GetSubmeshes();
				m_Submeshes.resize(submeshes.size());
				for (uint32_t i = 0; i < submeshes.size(); i++)
					m_Submeshes[i] = i;
			}
		}
	}
}