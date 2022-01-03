#pragma once

extern Valhalla::Application* Valhalla::CreateApplication(int argc, char** argv);
bool g_ApplicationRunning = true;

int main(int argc, char** argv)
{
	while (g_ApplicationRunning)
	{
		Valhalla::InitializeCore();
		Valhalla::Application* app = Valhalla::CreateApplication(argc, argv);
		app->Run();
		delete app;
		Valhalla::ShutdownCore();
	}
}