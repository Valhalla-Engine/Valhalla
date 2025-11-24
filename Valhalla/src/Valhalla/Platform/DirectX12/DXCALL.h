#pragma once
#include <winerror.h>
#define DXCALL(x) DXCall((HRESULT)(x), __FILE__, __LINE__)