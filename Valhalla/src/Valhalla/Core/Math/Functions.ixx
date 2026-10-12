module;
#include <DirectXMath.h>

export module Valhalla.Math:Functions;

import :Mat4x4;

export namespace Valhalla::Math
{
	Mat4x4 MatrixPerspectiveFovRH(float fovY, float aspect, float nearZ, float farZ)
	{
		return Mat4x4::FromXMMatrix(
			DirectX::XMMatrixPerspectiveFovRH(fovY, aspect, nearZ, farZ)
		);
	}

	Mat4x4 MatrixOrthographicRH(float width, float height, float nearZ, float farZ)
	{
		return Mat4x4::FromXMMatrix(
			DirectX::XMMatrixOrthographicRH(width, height, nearZ, farZ)
		);
	}

	float ConvertToRadians(float degrees)
	{
		return DirectX::XMConvertToRadians(degrees);
	}

	template<typename T>
	constexpr T Min(const T& a, const T& b)
	{
		return DirectX::XMMin(a, b);
	}
}