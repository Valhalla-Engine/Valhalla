module;

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

export module Valhalla.Yggdrasil.DirectX12.VD3D12;

import Valhalla.Core.Log;
import <system_error>;

namespace Valhalla::Yggdrasil
{
    export inline void DXCall(HRESULT hr, const char* file, int line)
    {
        if (FAILED(hr))
        {
            Log::CoreInfo(std::system_category().message(hr)
                + " \n File: {0}, Line: {1}", file, line);
        }
    }
}