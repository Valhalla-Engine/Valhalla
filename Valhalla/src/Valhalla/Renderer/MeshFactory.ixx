#include <DirectXMath.h>

export module Valhalla.Renderer.MeshFactory;

import Valhalla.Asset;

namespace Valhalla
{
	namespace Heimdall
	{
		class MeshFactory
		{
		public:
			static Mimir::AssetHandle CreateBox(const DirectX::XMFLOAT3& size);
			static Mimir::AssetHandle CreateSphere(float_t radius);
			static Mimir::AssetHandle CreateCapsule(float_t radius, float_t height);
		};
	}
}