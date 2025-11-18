export module Valhalla.Core.Types;

#include <DirectXMath.h>;

export namespace Valhalla
{
    // Fixed-width integer types
    export typedef char                int8_t;
    export typedef short               int16_t;
    export typedef int                 int32_t;
    export typedef long long           int64_t;

    export typedef unsigned char       uint8_t;
    export typedef unsigned short      uint16_t;
    export typedef unsigned int        uint32_t;
    export typedef unsigned long long  uint64_t;

    // Floating point and boolean
    export typedef float               float_t;
    export typedef double              double_t;
    export typedef bool                bool_t;

    // Byte alias
    export using byte = uint8_t;

    // Vector and matrix types from DirectXMath
    export using vec2 = DirectX::XMFLOAT2;
    export using vec3 = DirectX::XMFLOAT3;
    export using vec4 = DirectX::XMFLOAT4;

    export using mat3 = DirectX::XMFLOAT3X3;
    export using mat4 = DirectX::XMFLOAT4X4;
}