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

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window;}

		static inline Application& Get() { return *s_Instance; }

	private:
		
	};

	Application* CreateApplication();
}