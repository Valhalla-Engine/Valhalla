#pragma once
#include "Types.h"
#include "Base.h"
#include "TimeStep.h"
#include "Window.h"
#include "LayerStack.h"
#include "Timer.h"

#include "Events/ApplicationEvent.h"

#include "Valhalla/ImGui/ImGuiLayer.h"

namespace Valhalla
{
	struct ApplicationProps
	{
		std::string Name;
		uint32_t WindowWidth, WindowHeight;
	};

	class Application
	{
	public:
		Application(const ApplicationProps& props = {"Valhalla Engine", 1280, 720});
		virtual ~Application();

		void Run();

		virtual void OnInit() {}
		virtual void OnShutdown() {}
		virtual void OnUpdate(Timestep ts) {}

		virtual void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);
		void RenderImGui();

		inline Window& GetWindow() { return *m_Window; }

		static inline Application& Get() { return *s_Instance; }

		PerformanceProfiler* GetPerformanceProfiler() { return m_Profiler; }

	private:
		bool_t OnWindowResize(WindowResizeEvent& e);
		bool_t OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		bool_t m_Running = true, m_Minimized = false;
		LayerStack m_LayerStack;
		Timestep m_TimeStep;

		ImGuiLayer* m_ImGuiLayer;

		float_t m_LastFrameTime = 0.0f;

		PerformanceProfiler* m_Profiler = nullptr;

		static Application* s_Instance;
	};

	Application* CreateApplication();
}