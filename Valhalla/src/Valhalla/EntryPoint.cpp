#include <Valhalla/Entrypoint.h>

import Valhalla.Core;
import Valhalla.Core.Application;

namespace Valhalla
{
	int Main(int argc, char** argv)
	{
		InitializeCore();

		auto* app = CreateApplication(argc, argv);
		app->Run();
		delete app;

		ShutdownCore();

		return 0;
	}
}

#if defined(VH_DIST) && defined(VH_PLATFORM_WINDOWS)

#include <Windows.h>

int APIENTRY WinMain(
	HINSTANCE,
	HINSTANCE,
	PSTR,
	int)
{
	return Valhalla::Main(__argc, __argv);
}

#else

int main(int argc, char** argv)
{
	return Valhalla::Main(argc, argv);
}

#endif