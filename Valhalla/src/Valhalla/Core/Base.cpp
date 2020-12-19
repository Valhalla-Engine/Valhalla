#include "vhpch.h"
#include "Base.h"

#include "Log.h"

#define VALHALLA_BUILD_ID "v0.1a"

namespace Valhalla
{
	void VALHALLA_API InitializeCore()
	{
		Valhalla::Log::Init();

		VH_CORE_TRACE("Valhalla Engine {}", VALHALLA_BUILD_ID);
		VH_CORE_TRACE("Initializing...");
	}

	void VALHALLA_API ShutdownCore()
	{
		VH_CORE_TRACE("Shutting down...");
	}
}