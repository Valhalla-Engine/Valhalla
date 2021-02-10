#pragma once

extern Valhalla::Application* Valhalla::CreateApplication();



#ifdef VH_PLATFORM_WINDOWS
#include <Windows.h>

int main(int argv, char** argc)
{
	//Valhalla::InitializeCore();
	auto app = Valhalla::CreateApplication();
	app->Run();
	delete app;
	//Valhalla::ShutdownCore();
}
#endif
