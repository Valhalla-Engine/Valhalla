#pragma once

namespace Valhalla
{
	class Application;

	Application* CreateApplication(int argc, char** argv);
	int Main(int argc, char** argv);
}