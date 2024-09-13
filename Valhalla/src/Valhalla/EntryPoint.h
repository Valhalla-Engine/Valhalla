#pragma once

extern Valhalla::Application* Valhalla::CreateApplication(int argc, char** argv);


namespace Valhalla
{
	int Main(int argc, char** argv)
	{
		Valhalla::InitializeCore();
		auto app = Valhalla::CreateApplication(argc, argv);
		app->Run();
		delete app;
		Valhalla::ShutdownCore();

		return 0;
	}
}

#if VH_DIST && VH_PLATFORM_WINDOWS
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdLine, int cmdShow)
{
	return Valhalla::Main(__argc, __argc);
}
#else
int main(int argc, char** argv)
{
	return Valhalla::Main(argc, argv);
}
#endif