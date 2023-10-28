#pragma once

extern Valhalla::Application* Valhalla::CreateApplication(int argc, char** argv);

int main(int argc, char** argv)
{
	Valhalla::InitializeCore();
	auto app = Valhalla::CreateApplication(argc, argv);
	app->Run();
	delete app;
	Valhalla::ShutdownCore();
}
