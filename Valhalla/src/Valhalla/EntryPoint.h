#pragma once

extern Valhalla::Application* Valhalla::CreateApplication();

int main(int argv, char** argc)
{
	Valhalla::InitializeCore();
	auto app = Valhalla::CreateApplication();
	app->Run();
	delete app;
	Valhalla::ShutdownCore();
}
