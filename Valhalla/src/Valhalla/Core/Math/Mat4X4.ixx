module;

#include <compare>
#include <DirectXMath.h>

export module Valhalla.Math:Mat4x4;

export namespace Valhalla::Math
{
	struct Mat4x4
	{
		float m[4][4] =
		{
			{ 1.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		};

		constexpr Mat4x4() = default;

		constexpr Mat4x4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
			: m{
				{ m00, m01, m02, m03 },
				{ m10, m11, m12, m13 },
				{ m20, m21, m22, m23 },
				{ m30, m31, m32, m33 }
			}
		{}

		explicit Mat4x4(const DirectX::XMFLOAT4X4& matrix)
		{
			for (int row = 0; row < 4; ++row)
				for (int col = 0; col < 4; ++col)
					m[row][col] = matrix.m[row][col];
		}

		DirectX::XMFLOAT4X4 ToDirectX() const
		{
			return DirectX::XMFLOAT4X4(
				m[0][0], m[0][1], m[0][2], m[0][3],
				m[1][0], m[1][1], m[1][2], m[1][3],
				m[2][0], m[2][1], m[2][2], m[2][3],
				m[3][0], m[3][1], m[3][2], m[3][3]
			);
		}

		DirectX::XMMATRIX ToXMMatrix() const
		{
			const auto dxMatrix = ToDirectX();
			return DirectX::XMLoadFloat4x4(&dxMatrix);
		}

		static Mat4x4 FromXMMatrix(DirectX::FXMMATRIX matrix)
		{
			DirectX::XMFLOAT4X4 result;
			DirectX::XMStoreFloat4x4(&result, matrix);

			return Mat4x4(result);
		}

		Mat4x4 operator+(const Mat4x4& rhs) const
		{
			return FromXMMatrix(
				ToXMMatrix() + rhs.ToXMMatrix()
			);
		}

		Mat4x4 operator-(const Mat4x4& rhs) const
		{
			return FromXMMatrix(
				ToXMMatrix() - rhs.ToXMMatrix()
			);
		}

		Mat4x4 operator*(const Mat4x4& rhs) const
		{
			return FromXMMatrix(
				DirectX::XMMatrixMultiply(
					ToXMMatrix(),
					rhs.ToXMMatrix()
				)
			);
		}

		Mat4x4 operator*(float scalar) const
		{
			return FromXMMatrix(
				ToXMMatrix() * scalar
			);
		}

		Mat4x4 operator/(float scalar) const
		{
			return FromXMMatrix(
				ToXMMatrix() / scalar
			);
		}

		Mat4x4& operator+=(const Mat4x4& rhs)
		{
			*this = *this + rhs;
			return *this;
		}

		Mat4x4& operator-=(const Mat4x4& rhs)
		{
			*this = *this - rhs;
			return *this;
		}

		Mat4x4& operator*=(const Mat4x4& rhs)
		{
			*this = *this * rhs;
			return *this;
		}

		Mat4x4& operator*=(float scalar)
		{
			*this = *this * scalar;
			return *this;
		}

		Mat4x4& operator/=(float scalar)
		{
			*this = *this / scalar;
			return *this;
		}

		constexpr float* operator[](int row)
		{
			return m[row];
		}

		constexpr const float* operator[](int row) const
		{
			return m[row];
		}

		constexpr bool operator==(const Mat4x4&) const = default;

		static Mat4x4 Identity()
		{
			return FromXMMatrix(
				DirectX::XMMatrixIdentity()
			);
		}

		static constexpr Mat4x4 Zero()
		{
			return
			{
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0
			};
		}

		Mat4x4 Transpose() const
		{
			return FromXMMatrix(
				DirectX::XMMatrixTranspose(ToXMMatrix())
			);
		}

		Mat4x4 Inverse() const
		{
			return FromXMMatrix(
				DirectX::XMMatrixInverse(
					nullptr,
					ToXMMatrix()
				)
			);
		}
	};

	inline Mat4x4 operator*(float scalar, const Mat4x4& matrix)
	{
		return matrix * scalar;
	}
}