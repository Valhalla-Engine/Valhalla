#pragma once

extern Valhalla::Application* Valhalla::CreateApplication();



#ifdef VH_PLATFORM_WINDOWS
#include <Windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	Valhalla::InitializeCore();
	auto app = Valhalla::CreateApplication();
	app->Run();
	delete app;
	Valhalla::ShutdownCore();
}
#endif

#ifdef VH_PLATFORM_OTHER
int main(int argc, char** argv)
{
	Valhalla::Log::Init();
	auto app = Valhalla::CreateApplication();
	app->Run();
	delete app;
}

#endif