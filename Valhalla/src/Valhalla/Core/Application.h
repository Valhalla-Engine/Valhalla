#pragma once
#include "Types.h"
#include "Base.h"

namespace Valhalla
{
	class Application
	{
	public:
		Application();
		~Application();

		void Run();

		
	};

	Application* CreateApplication();
}