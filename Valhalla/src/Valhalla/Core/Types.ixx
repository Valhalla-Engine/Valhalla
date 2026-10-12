export module Valhalla.Core.Types;


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
}