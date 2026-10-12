module;

#include <compare>
#include <DirectXMath.h>

export module Valhalla.Math:Mat3x3;

export namespace Valhalla::Math
{
	struct Mat3x3
	{
		float m[3][3] =
		{
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f }
		};

		constexpr Mat3x3() = default;

		constexpr Mat3x3(
			float m00, float m01, float m02,
			float m10, float m11, float m12,
			float m20, float m21, float m22)
			: m{
				{ m00, m01, m02 },
				{ m10, m11, m12 },
				{ m20, m21, m22 }
			}
		{}

		explicit Mat3x3(const DirectX::XMFLOAT3X3& matrix)
		{
			for (int row = 0; row < 3; ++row)
				for (int col = 0; col < 3; ++col)
					m[row][col] = matrix.m[row][col];
		}

		DirectX::XMFLOAT3X3 ToDirectX() const
		{
			return DirectX::XMFLOAT3X3(
				m[0][0], m[0][1], m[0][2],
				m[1][0], m[1][1], m[1][2],
				m[2][0], m[2][1], m[2][2]
			);
		}

		DirectX::XMMATRIX ToXMMatrix() const
		{
			auto dxMatrix = ToDirectX();
			return DirectX::XMLoadFloat3x3(&dxMatrix);
		}

		static Mat3x3 FromXMMatrix(DirectX::FXMMATRIX matrix)
		{
			DirectX::XMFLOAT3X3 result;
			DirectX::XMStoreFloat3x3(&result, matrix);

			return Mat3x3(result);
		}

		Mat3x3 operator*(const Mat3x3& rhs) const
		{
			const auto lhsMatrix = ToXMMatrix();
			const auto rhsMatrix = rhs.ToXMMatrix();

			return FromXMMatrix(
				DirectX::XMMatrixMultiply(lhsMatrix, rhsMatrix)
			);
		}

		Mat3x3& operator*=(const Mat3x3& rhs)
		{
			*this = *this * rhs;
			return *this;
		}

		constexpr Mat3x3 operator+(const Mat3x3& rhs) const
		{
			return
			{
				m[0][0] + rhs.m[0][0],
				m[0][1] + rhs.m[0][1],
				m[0][2] + rhs.m[0][2],

				m[1][0] + rhs.m[1][0],
				m[1][1] + rhs.m[1][1],
				m[1][2] + rhs.m[1][2],

				m[2][0] + rhs.m[2][0],
				m[2][1] + rhs.m[2][1],
				m[2][2] + rhs.m[2][2]
			};
		}

		constexpr Mat3x3 operator-(const Mat3x3& rhs) const
		{
			return
			{
				m[0][0] - rhs.m[0][0],
				m[0][1] - rhs.m[0][1],
				m[0][2] - rhs.m[0][2],

				m[1][0] - rhs.m[1][0],
				m[1][1] - rhs.m[1][1],
				m[1][2] - rhs.m[1][2],

				m[2][0] - rhs.m[2][0],
				m[2][1] - rhs.m[2][1],
				m[2][2] - rhs.m[2][2]
			};
		}

		constexpr Mat3x3 operator*(float scalar) const
		{
			return
			{
				m[0][0] * scalar,
				m[0][1] * scalar,
				m[0][2] * scalar,

				m[1][0] * scalar,
				m[1][1] * scalar,
				m[1][2] * scalar,

				m[2][0] * scalar,
				m[2][1] * scalar,
				m[2][2] * scalar
			};
		}

		constexpr float* operator[](int row)
		{
			return m[row];
		}

		constexpr const float* operator[](int row) const
		{
			return m[row];
		}

		constexpr bool operator==(const Mat3x3&) const = default;

		static constexpr Mat3x3 Identity()
		{
			return {};
		}

		static constexpr Mat3x3 Zero()
		{
			return
			{
				0, 0, 0,
				0, 0, 0,
				0, 0, 0
			};
		}
	};

	constexpr Mat3x3 operator*(float scalar, const Mat3x3& matrix)
	{
		return matrix * scalar;
	}
}