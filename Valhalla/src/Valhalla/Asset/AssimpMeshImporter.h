#pragma once

#include "Valhalla/Renderer/Mesh.h"

#include <filesystem>

namespace Valhalla
{
	namespace Heimdall
	{
		class MeshSource;
	}

	namespace Mimir
	{
		class AssimpMeshImporter
		{
		public:
			AssimpMeshImporter(const std::filesystem::path& path);

			Ref<Heimdall::MeshSource> ImportToMeshSource();
		private:
			void TraverseNodes(Ref<Heimdall::MeshSource> meshSource, void* assimpNode, uint32_t nodeIndex, const DirectX::XMFLOAT4X4& parentTransform = DirectX::XMFLOAT4X4Identity(), uint32_t level = 0);
		private:
			const std::filesystem::path m_Path;
		};
	}
}