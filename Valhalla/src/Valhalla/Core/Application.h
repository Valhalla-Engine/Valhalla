#pragma once
#include "Type.h"
#include "Base.h"


namespace Valhalla
{
	class Application
	{
	public:
		Application();
		~Application();

		void Run();
	private:
		
	};

	Application* CreateApplication();
}