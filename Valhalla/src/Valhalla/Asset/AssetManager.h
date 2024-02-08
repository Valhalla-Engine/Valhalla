#pragma once


#include "Valhalla/Core/Hash.h"
#include "Valhalla/Core/Application.h"
#include "Valhalla/Core/Timer.h"

#include "Asset.h"

namespace Valhalla
{
	namespace Mimir
	{
		class AssetManager
		{
		public:

			static bool IsAssetHandleValid(AssetHandle assetHandle);
		};
	}
}