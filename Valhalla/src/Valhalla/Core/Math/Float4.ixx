module;
#include <compare>
#include <DirectXMath.h>

export module Valhalla.Math:Vec4;

export namespace Valhalla::Math
{
	struct Float4
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;

		constexpr Float4() = default;

		constexpr Float4(float x, float y, float z, float w)
			: x(x), y(y), z(z), w(w)
		{}

		constexpr Float4 operator+(const Float4& rhs) const
		{
			return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
		}

		constexpr Float4 operator-(const Float4& rhs) const
		{
			return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
		}

		constexpr Float4 operator*(float scalar) const
		{
			return { x * scalar, y * scalar, z * scalar, w * scalar };
		}

		constexpr Float4 operator/(float scalar) const
		{
			return { x / scalar, y / scalar, z / scalar, w / scalar };
		}

		constexpr Float4& operator+=(const Float4& rhs)
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			w += rhs.w;
			return *this;
		}

		constexpr Float4& operator-=(const Float4& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			w -= rhs.w;
			return *this;
		}

		constexpr Float4& operator*=(float scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			w *= scalar;
			return *this;
		}

		constexpr Float4& operator/=(float scalar)
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			w /= scalar;
			return *this;
		}

		constexpr Float4 operator-() const
		{
			return { -x, -y, -z, -w };
		}

		constexpr bool operator==(const Float4&) const = default;

		DirectX::XMVECTOR ToXMVector() const
		{
			return DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4*>(this));
		}

		static Float4 FromXM(const DirectX::XMVECTOR& vec)
		{
			DirectX::XMFLOAT4 result;
			DirectX::XMStoreFloat4(&result, vec);
			return *reinterpret_cast<Float4*>(&result);
		}
	};


	constexpr Float4 operator*(float scalar, const Float4& vector)
	{
		return vector * scalar;
	}
}