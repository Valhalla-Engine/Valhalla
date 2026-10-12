
export module Valhalla.Renderer.MeshFactory;

import Valhalla.Asset;
import Valhalla.Core.Types;
import Valhalla.Math;

namespace Valhalla
{
	namespace Heimdall
	{
		class MeshFactory
		{
		public:
			static Mimir::AssetHandle CreateBox(const Math::Float3& size);
			static Mimir::AssetHandle CreateSphere(float_t radius);
			static Mimir::AssetHandle CreateCapsule(float_t radius, float_t height);
		};
	}
}