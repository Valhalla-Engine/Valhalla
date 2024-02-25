#pragma once

#include <DirectXMath.h>


namespace DirectX
{
	DirectX::XMFLOAT4X4 XM_CALLCONV XMFLOAT4X4Identity()
	{
		XMFLOAT4X4 matrix;

		matrix.m[0][0] = 1.0f; matrix.m[0][1] = 0.0f; matrix.m[0][2] = 0.0f; matrix.m[0][3] = 0.0f;
		matrix.m[1][0] = 0.0f; matrix.m[1][1] = 1.0f; matrix.m[1][2] = 0.0f; matrix.m[1][3] = 0.0f;
		matrix.m[2][0] = 0.0f; matrix.m[2][1] = 0.0f; matrix.m[2][2] = 1.0f; matrix.m[2][3] = 0.0f;
		matrix.m[3][0] = 0.0f; matrix.m[3][1] = 0.0f; matrix.m[3][2] = 0.0f; matrix.m[3][3] = 1.0f;

		return matrix;
	}
}
