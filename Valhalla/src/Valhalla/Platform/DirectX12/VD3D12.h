#pragma once
#include "dxgi.h"

#include "Valhalla/Core/Log.h"

#include <iostream>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>
#include <d3dcommon.h>
#include "d3dx12.h"
#ifdef VH_DEBUG
#include <dxgidebug.h>
#pragma comment (lib, "dxguid.lib")
#endif

#pragma comment (lib, "d3d12.lib")
#pragma comment (lib, "D3DCompiler.lib")
#pragma comment (lib, "dxgi.lib")

namespace Valhalla
{
	namespace Yggdrasil
	{
#define DXCALL(x)																					\
if ( FAILED((HRESULT) x))																			\
{																									\
	VH_CORE_INFO(std::system_category().message(x) + " \n File: {0}, Line: {1}",__FILE__, __LINE__ );	\
}

	}
}