#pragma once

#include "Texture.h"

namespace Valhalla
{
	namespace Heimdall
	{
		class Environment : public Mimir::Asset
		{
		public:

			Ref<TextureCube> RadianceMap;
			Ref<TextureCube> IrradianceMap;

			Environment() = default;
			Environment(const Ref<TextureCube>& radianceMap, const Ref<TextureCube>& irradianceMap)
				: RadianceMap(radianceMap), IrradianceMap(irradianceMap) {}

			static Mimir::AssetType GetStaticType() { return Mimir::AssetType::EnvMap; }
			virtual Mimir::AssetType GetAssetType() const override { return GetStaticType(); }
		};
	}
}