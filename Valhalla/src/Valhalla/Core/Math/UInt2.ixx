export module Valhalla.Math:UVec2;
import Valhalla.Core.Types;

export namespace Valhalla::Math
{
	struct UInt2
	{
		uint32_t x = 0;
		uint32_t y = 0;

		constexpr UInt2() = default;

		constexpr UInt2(uint32_t x, uint32_t y)
			: x(x), y(y)
		{}

		constexpr UInt2 operator+(const UInt2& rhs) const
		{
			return { x + rhs.x, y + rhs.y };
		}

		constexpr UInt2 operator-(const UInt2& rhs) const
		{
			return { x - rhs.x, y - rhs.y };
		}

		constexpr UInt2 operator*(uint32_t scalar) const
		{
			return { x * scalar, y * scalar };
		}

		constexpr UInt2 operator/(uint32_t scalar) const
		{
			return { x / scalar, y / scalar };
		}

		constexpr UInt2& operator+=(const UInt2& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		constexpr UInt2& operator-=(const UInt2& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		constexpr UInt2& operator*=(uint32_t scalar)
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}

		constexpr UInt2& operator/=(uint32_t scalar)
		{
			x /= scalar;
			y /= scalar;
			return *this;
		}

		constexpr bool operator==(const UInt2&) const = default;
	};

	constexpr UInt2 operator*(uint32_t scalar, const UInt2& vector)
	{
		return vector * scalar;
	}
}