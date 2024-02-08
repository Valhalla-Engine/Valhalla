#pragma once

#include "Valhalla/Core/Base.h"
#include "Valhalla/Core/Log.h"

namespace Valhalla
{
	namespace Mimir
	{
		enum class AssetFlag : uint16_t
		{
			None = 0,
			Missing = BIT(0),
			Invalid = BIT(1)
		};

		enum class AssetType : uint16_t
		{
			None = 0,
			Scene,
			Prefab,
			Mesh,
			StaticMesh,
			MeshSource,
			Texture,
			EnvMap
		};

		namespace Utils
		{
			inline AssetType AssetTypeFromString(const std::string& assetType)
			{
				if (assetType == "None")			return AssetType::None;
				if (assetType == "Scene")			return AssetType::Scene;
				if (assetType == "Prefab")			return AssetType::Prefab;
				if (assetType == "Mesh")			return AssetType::Mesh;
				if (assetType == "StaticMesh")		return AssetType::StaticMesh;
				if (assetType == "MeshSource")		return AssetType::MeshSource;
				if (assetType == "Texture")			return AssetType::Texture;
				if (assetType == "EnvMap")			return AssetType::EnvMap;

				VH_CORE_ASSERT(false, "Unknown Asset Type");
				return AssetType::None;
			}

			inline const char* AssetTypeToString(AssetType assetType)
			{
				switch (assetType)
				{
					case AssetType::None:			return "None";
					case AssetType::Scene:			return "Scene";
					case AssetType::Prefab:			return "Prefab";
					case AssetType::Mesh:			return "Mesh";
					case AssetType::StaticMesh:		return "StaticMesh";
					case AssetType::MeshSource:		return "MeshSource";
					case AssetType::Texture:		return "Texture";
					case AssetType::EnvMap:			return "EnvMap";
				}

				VH_CORE_ASSERT(false, "Unknown Asset Type");
				return "None";
			}
		}
	}
}