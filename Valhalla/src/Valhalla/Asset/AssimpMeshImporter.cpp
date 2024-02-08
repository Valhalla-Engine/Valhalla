#include "vhpch.h"
#include "AssimpMeshImporter.h"

#include "Valhalla/Utilities/AssimpLogStream.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

namespace Valhalla
{
	namespace Mimir
	{
		static const uint32_t s_MeshImportFlags =
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_SortByPType |
			aiProcess_GenNormals |
			aiProcess_GenUVCoords |
			aiProcess_OptimizeMeshes |
			aiProcess_JoinIdenticalVertices |
			aiProcess_LimitBoneWeights |
			aiProcess_ValidateDataStructure |
			aiProcess_GlobalScale;

		namespace Utils
		{
			DirectX::XMFLOAT4X4 DirectXMat4FromAiMatrix4X4(const aiMatrix4x4& matrix)
			{
				DirectX::XMFLOAT4X4 result;
				result.m[0][0] = matrix.a1; result.m[0][1] = matrix.a2; result.m[0][2] = matrix.a3; result.m[0][3] = matrix.a4;
				result.m[1][0] = matrix.b1; result.m[1][1] = matrix.b2; result.m[1][2] = matrix.b3; result.m[1][3] = matrix.b4;
				result.m[2][0] = matrix.c1; result.m[2][1] = matrix.c2; result.m[2][2] = matrix.c3; result.m[2][3] = matrix.c4;
				result.m[3][0] = matrix.d1; result.m[3][1] = matrix.d2; result.m[3][2] = matrix.d3; result.m[3][3] = matrix.d4;
				return result;
			}
		}

		AssimpMeshImporter::AssimpMeshImporter(const std::filesystem::path& path)
			: m_Path(path)
		{
			AssimpLogStream::Initialize();
		}

		Ref<Heimdall::MeshSource> AssimpMeshImporter::ImportToMeshSource()
		{
			Ref<Heimdall::MeshSource> meshSource = Ref<Heimdall::MeshSource>::Create();

			VH_CORE_INFO_TAG("Mesh", "Loading mesh: {0}", m_Path.string());

			Assimp::Importer importer;
			importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

			const aiScene* scene = importer.ReadFile(m_Path.string(), s_MeshImportFlags);
			if (!scene)
			{
				VH_CORE_ERROR_TAG("Mesh", "Failed to load mesh file: {0}", m_Path.string());
				meshSource->SetFlag(Mimir::AssetFlag::Invalid);
				return nullptr;
			}

			if (scene->HasMeshes())
			{
				uint32_t vertexCount = 0;
				uint32_t indexCount = 0;

				meshSource->m_BoundingBox.Min = {FLT_MAX, FLT_MAX, FLT_MAX};
				meshSource->m_BoundingBox.Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

				meshSource->m_Submeshes.reserve(scene->mNumMeshes);
				for (unsigned m = 0; m < scene->mNumMeshes; m++)
				{
					aiMesh* mesh = scene->mMeshes[m];

					Heimdall::Submesh& submesh = meshSource->m_Submeshes.emplace_back();
					submesh.BaseVertex = vertexCount;
					submesh.BaseIndex = indexCount;
					submesh.VertexCount = mesh->mNumVertices;
					submesh.IndexCount = mesh->mNumFaces * 3;
					submesh.MeshName = mesh->mName.C_Str();

					vertexCount += mesh->mNumVertices;
					indexCount += submesh.IndexCount;

					VH_CORE_ASSERT(mesh->HasPositions(), "Meshes require positions.");
					VH_CORE_ASSERT(mesh->HasNormals(), "Meshes require normals");

					auto& aabb = submesh.BoundingBox;
					aabb.Min = {FLT_MAX, FLT_MAX, FLT_MAX};
					aabb.Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

					// Vertices
					for (size_t i = 0; i < mesh->mNumVertices; i++)
					{
						Heimdall::Vertex vertex;
						vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
						vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
						aabb.Min.x = std::min<float>(vertex.Position.x, aabb.Min.x);
						aabb.Min.y = std::min<float>(vertex.Position.y, aabb.Min.y);
						aabb.Min.z = std::min<float>(vertex.Position.z, aabb.Min.z);
						aabb.Max.x = std::max<float>(vertex.Position.x, aabb.Max.x);
						aabb.Max.y = std::max<float>(vertex.Position.y, aabb.Max.y);
						aabb.Max.z = std::max<float>(vertex.Position.z, aabb.Max.z);

						if (mesh->HasTangentsAndBitangents())
						{
							vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
							vertex.BiNormal = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
						}

						if (mesh->HasTextureCoords(0))
						{
							vertex.TexCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
						}

						meshSource->m_Vertices.push_back(vertex);
					}

					// Indices
					for (size_t i = 0; i < mesh->mNumFaces; i++)
					{
						VH_CORE_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices");
						Heimdall::Index index = { mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] };
						meshSource->m_Indices.push_back(index);

						meshSource->m_TriangleCache[m].emplace_back(meshSource->m_Vertices[index.V1 + submesh.BaseVertex], meshSource->m_Vertices[index.V2 + submesh.BaseVertex], meshSource->m_Vertices[index.V3 + submesh.BaseVertex]);

					}

					Heimdall::MeshNode& rootNode = meshSource->m_Nodes.emplace_back();
					TraverseNodes(meshSource, scene->mRootNode, 0);

					for (const auto& submesh : meshSource->m_Submeshes)
					{
						AABB transformedSubmeshAABB = submesh.BoundingBox;
						DirectX::XMFLOAT4 transMin = { transformedSubmeshAABB.Min.x, transformedSubmeshAABB.Min.y, transformedSubmeshAABB.Min.z, 1.0f };
						DirectX::XMFLOAT4 transMax = { transformedSubmeshAABB.Max.x, transformedSubmeshAABB.Max.y, transformedSubmeshAABB.Max.z, 1.0f };
						DirectX::XMFLOAT3 min;
						DirectX::XMFLOAT3 max;
						DirectX::XMStoreFloat3(&min, DirectX::XMVector4Transform(DirectX::XMLoadFloat4(&transMin), DirectX::XMLoadFloat4x4(&submesh.transform)));
						DirectX::XMStoreFloat3(&max, DirectX::XMVector4Transform(DirectX::XMLoadFloat4(&transMax), DirectX::XMLoadFloat4x4(&submesh.transform)));

						meshSource->m_BoundingBox.Min.x = std::min<float>(meshSource->m_BoundingBox.Min.x, min.x);
						meshSource->m_BoundingBox.Min.y = std::min<float>(meshSource->m_BoundingBox.Min.y, min.y);
						meshSource->m_BoundingBox.Min.z = std::min<float>(meshSource->m_BoundingBox.Min.z, min.z);
						meshSource->m_BoundingBox.Max.x = std::min<float>(meshSource->m_BoundingBox.Max.x, max.x);
						meshSource->m_BoundingBox.Max.y = std::min<float>(meshSource->m_BoundingBox.Max.y, max.y);
						meshSource->m_BoundingBox.Max.z = std::min<float>(meshSource->m_BoundingBox.Max.z, max.z);

					}

					if (meshSource->m_Vertices.size())
						meshSource->m_VertexBuffer = Heimdall::VertexBuffer::Create(meshSource->m_Vertices.data(), (uint32_t)(meshSource->m_Vertices.size() * sizeof(Heimdall::Vertex)));

					if (meshSource->m_Indices.size())
						meshSource->m_IndexBuffer = Heimdall::IndexBuffer::Create(meshSource->m_Indices.data(), (uint32_t)(meshSource->m_Indices.size() * sizeof(Heimdall::Index)));

					return meshSource;
				}
			}
		}

		void AssimpMeshImporter::TraverseNodes(Ref<Heimdall::MeshSource> meshSource, void* assimpNode, uint32_t nodeIndex, const DirectX::XMFLOAT4X4& parentTransform, uint32_t level)
		{
			aiNode* aNode = (aiNode*)assimpNode;

			Heimdall::MeshNode& node = meshSource->m_Nodes[nodeIndex];
			node.Name = aNode->mName.C_Str();
			node.LocalTransform = Utils::DirectXMat4FromAiMatrix4X4(aNode->mTransformation);

			DirectX::XMFLOAT4X4 transform;
			DirectX::XMStoreFloat4x4(&transform, DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&parentTransform), DirectX::XMLoadFloat4x4(&node.LocalTransform)));
			for (uint32_t i = 0; i < aNode->mNumMeshes; i++)
			{
				uint32_t submeshIndex = aNode->mMeshes[i];
				auto& submesh = meshSource->m_Submeshes[submeshIndex];
				submesh.NodeName = aNode->mName.C_Str();
				submesh.transform = transform;
				submesh.LocalTransform = node.LocalTransform;


				node.Submeshes.push_back(submeshIndex);
			}

			uint32_t parentNodeIndex = (uint32_t)meshSource->m_Nodes.size() - 1;
			node.Children.resize(aNode->mNumChildren);
			for (uint32_t i = 0; i < aNode->mNumChildren; i++)
			{
				Heimdall::MeshNode& child = meshSource->m_Nodes.emplace_back();
				size_t childIndex = meshSource->m_Nodes.size() - 1;
				child.Parent = parentNodeIndex;
				meshSource->m_Nodes[nodeIndex].Children[i] = childIndex;
				TraverseNodes(meshSource, aNode->mChildren[i], uint32_t(childIndex), transform, level + 1);
			}
		}
	}
}