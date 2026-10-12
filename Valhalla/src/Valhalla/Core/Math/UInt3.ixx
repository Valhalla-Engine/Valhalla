export module Valhalla.Math:UVec3;
import Valhalla.Core.Types;
export namespace Valhalla::Math
{
	struct UInt3
	{
		uint32_t x = 0;
		uint32_t y = 0;
		uint32_t z = 0;

		constexpr UInt3() = default;

		constexpr UInt3(uint32_t x, uint32_t y, uint32_t z)
			: x(x), y(y), z(z)
		{}

		constexpr UInt3 operator+(const UInt3& rhs) const
		{
			return { x + rhs.x, y + rhs.y, z + rhs.z };
		}

		constexpr UInt3 operator-(const UInt3& rhs) const
		{
			return { x - rhs.x, y - rhs.y, z - rhs.z };
		}

		constexpr UInt3 operator*(uint32_t scalar) const
		{
			return { x * scalar, y * scalar, z * scalar };
		}

		constexpr UInt3 operator/(uint32_t scalar) const
		{
			return { x / scalar, y / scalar, z / scalar };
		}

		constexpr UInt3& operator+=(const UInt3& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}

		constexpr UInt3& operator-=(const UInt3& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}

		constexpr UInt3& operator*=(uint32_t scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		constexpr UInt3& operator/=(uint32_t scalar)
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}

		constexpr bool operator==(const UInt3&) const = default;
	};

	constexpr UInt3 operator*(uint32_t scalar, const UInt3& vector)
	{
		return vector * scalar;
	}
}