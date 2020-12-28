#pragma once
#include "Valhalla/Core/Base.h"
#include "Valhalla/Core/Window.h"
#include "Valhalla/Core/LayerStack.h"
#include "Valhalla/Core/Events/ApplicationEvent.h"

namespace Valhalla
{
	struct ApplicationProps
	{
		std::string Name;
		uint32_t WindowWidth, WindowHeight;
	};

	class VALHALLA_API Application
	{
	public:
		Application(const ApplicationProps& props = {"Valhalla Engine", 1280, 720});
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }

		static inline Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool Application::OnWindowResize(WindowResizeEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
		bool m_Running = true, m_Minimized = false;

		static Application* s_Instance;
	};

	Application* CreateApplication();
}