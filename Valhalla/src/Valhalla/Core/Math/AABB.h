#pragma once

#include <DirectXMath.h>

namespace Valhalla
{
	struct AABB
	{
		DirectX::XMFLOAT3 Min, Max;

		AABB()
			:Min(0.0f, 0.0f, 0.0f), Max(0.0f, 0.0f, 0.0f) {}

		AABB(const DirectX::XMFLOAT3& min, const DirectX::XMFLOAT3& max)
			: Min(min), Max(max) {}
	};
}