export module Valhalla.Core.Math.AABB;
import Valhalla.Math;


export namespace Valhalla
{
	struct AABB
	{
		Math::Float3 Min, Max;

		AABB()
			:Min(0.0f, 0.0f, 0.0f), Max(0.0f, 0.0f, 0.0f) {}

		AABB(const Math::Float3& min, const Math::Float3& max)
			: Min(min), Max(max) {}
	};
}