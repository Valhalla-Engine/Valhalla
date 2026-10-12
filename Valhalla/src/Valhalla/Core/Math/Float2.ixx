export module Valhalla.Math:Vec2;

export namespace Valhalla::Math
{
	struct Float2
	{
		float x = 0.0f;
		float y = 0.0f;

		constexpr Float2() = default;

		constexpr Float2(float x, float y)
			: x(x), y(y)
		{}

		constexpr Float2 operator+(const Float2& rhs) const
		{
			return { x + rhs.x, y + rhs.y };
		}

		constexpr Float2 operator-(const Float2& rhs) const
		{
			return { x - rhs.x, y - rhs.y };
		}

		constexpr Float2 operator*(float scalar) const
		{
			return { x * scalar, y * scalar };
		}

		constexpr Float2 operator/(float scalar) const
		{
			return { x / scalar, y / scalar };
		}

		constexpr Float2& operator+=(const Float2& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		constexpr Float2& operator-=(const Float2& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		constexpr Float2& operator*=(float scalar)
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}

		constexpr Float2& operator/=(float scalar)
		{
			x /= scalar;
			y /= scalar;
			return *this;
		}

		constexpr Float2 operator-() const
		{
			return { -x, -y };
		}

		constexpr bool operator==(const Float2&) const = default;
	};

	constexpr Float2 operator*(float scalar, const Float2& vector)
	{
		return vector * scalar;
	}
}