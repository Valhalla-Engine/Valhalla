module;
#include "DirectXMath.h"

export module Valhalla.Math:Vec3;

export namespace Valhalla::Math
{
	struct Float3
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		constexpr Float3() = default;

		constexpr Float3(float x, float y, float z)
			: x(x), y(y), z(z)
		{}

		constexpr Float3 operator+(const Float3& rhs) const
		{
			return { x + rhs.x, y + rhs.y, z + rhs.z };
		}

		constexpr Float3 operator-(const Float3& rhs) const
		{
			return { x - rhs.x, y - rhs.y, z - rhs.z };
		}

		constexpr Float3 operator*(float scalar) const
		{
			return { x * scalar, y * scalar, z * scalar };
		}

		constexpr Float3 operator/(float scalar) const
		{
			return { x / scalar, y / scalar, z / scalar };
		}

		constexpr Float3& operator+=(const Float3& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}

		constexpr Float3& operator-=(const Float3& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}

		constexpr Float3& operator*=(float scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		constexpr Float3& operator/=(float scalar)
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}

		constexpr Float3 operator-() const
		{
			return { -x, -y, -z };
		}

		constexpr bool operator==(const Float3&) const = default;

		

		DirectX::XMVECTOR ToXM() const
		{
			return DirectX::XMVectorSet(x, y, z, 0.0f);
		}

		static Float3 FromXM(DirectX::FXMVECTOR v)
		{
			DirectX::XMFLOAT3 out;
			DirectX::XMStoreFloat3(&out, v);
			return { out.x, out.y, out.z };
		}
	};

	constexpr Float3 operator*(float scalar, const Float3& vector)
	{
		return vector * scalar;
	}
}