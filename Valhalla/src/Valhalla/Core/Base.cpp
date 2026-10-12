
module;
#include <iostream>

module Valhalla.Core;

import Valhalla.Core.Log;


#define VALHALLA_BUILD_ID "v0.1a"

namespace Valhalla
{
	void InitializeCore()
	{
		Log::Init();

		Log::CoreTrace("Valhalla Engine {0}", VALHALLA_BUILD_ID);
		Log::CoreTrace("Initializing...");
	}

	void ShutdownCore()
	{
		Log::CoreTrace("Shutting down...");
	}
}