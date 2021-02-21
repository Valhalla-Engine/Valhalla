#pragma once

#include <DirectXMath.h>

namespace Valhalla
{
	typedef char				int8_t;
	typedef short				int16_t;
	typedef int					int32_t;
	typedef long long			int64_t;

	typedef unsigned char		uint8_t;
	typedef unsigned short		uint16_t;
	typedef unsigned int		uint32_t;
	typedef unsigned long long	uint64_t;

	typedef float				float_t;
	typedef double				double_t;
	typedef bool				bool_t;

	using byte = uint8_t;

	using vec2 = DirectX::XMFLOAT2;
	using vec3 = DirectX::XMFLOAT3;
	using vec4 = DirectX::XMFLOAT4;

	using mat3 = DirectX::XMFLOAT3X3;
	using mat4 = DirectX::XMFLOAT4X4;
}