export module Valhalla.Asset.AssetType;

import Valhalla.Core.Types;
import Valhalla.Core.Log;
import Valhalla.Core.Assert;
import Valhalla.Core;

import <string>;

export namespace Valhalla
{
	export namespace Mimir
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

		export namespace Utils
		{
			inline AssetType AssetTypeFromString(std::string_view assetType)
			{
				if (assetType == "None")			return AssetType::None;
				if (assetType == "Scene")			return AssetType::Scene;
				if (assetType == "Prefab")			return AssetType::Prefab;
				if (assetType == "Mesh")			return AssetType::Mesh;
				if (assetType == "StaticMesh")		return AssetType::StaticMesh;
				if (assetType == "MeshSource")		return AssetType::MeshSource;
				if (assetType == "Texture")			return AssetType::Texture;
				if (assetType == "EnvMap")			return AssetType::EnvMap;

				Assert(false, "Unknown Asset Type");
				return AssetType::None;
			}

			inline const char* AssetTypeToString(AssetType type)
			{
				switch (type)
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
				Assert(false, "Unknown Asset Type");
				return "None";
			}
		}
	}
}