export module Valhalla.Renderer.SceneEnvironment;

import Valhalla.Asset;
import Valhalla.Renderer.Texture;
import Valhalla.Core.Ref;

export namespace Valhalla
{
	export namespace Heimdall
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