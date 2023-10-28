#pragma once
#include "Types.h"
#include "Base.h"
#include "TimeStep.h"
#include "Window.h"
#include "LayerStack.h"
#include "Timer.h"

#include "Events/ApplicationEvent.h"

#include "Valhalla/ImGui/ImGuiLayer.h"

#include <queue>

namespace Valhalla
{
	struct ApplicationProps
	{
		std::string Name;
		uint32_t WindowWidth, WindowHeight;
	};

	struct ApplicationSpecification
	{
		std::string Name = "Valhalla";
		uint32_t WindowWidth = 1600, WindowHeight = 900;
		bool WindowDecorated = false;
		bool Fullscreen = false;
		bool VSync = true;
		bool StartMaximized = true;
		bool Resizable = true;
		bool EnableImGui = true;
	};

	class Application
	{
		using EventCallBackFn = std::function<void(Event&)>;

	public:
		Application(const ApplicationSpecification& specifications);
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

		void AddEventCallback(const EventCallBackFn& eventCallback) { m_EventCallbacks.push_back(eventCallback); }

		inline Window& GetWindow() { return *m_Window; }

		static inline Application& Get() { return *s_Instance; }

		PerformanceProfiler* GetPerformanceProfiler() { return m_Profiler; }

	private:
		void ProcessEvents();

		bool_t OnWindowResize(WindowResizeEvent& e);
		bool_t OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		ApplicationSpecification m_Specification;
		bool_t m_Running = true, m_Minimized = false;
		LayerStack m_LayerStack;
		Timestep m_TimeStep;

		ImGuiLayer* m_ImGuiLayer;

		float_t m_LastFrameTime = 0.0f;

		PerformanceProfiler* m_Profiler = nullptr;

		std::mutex m_EventQueueMutex;
		std::queue<std::function<void()>> m_EventQueue;
		std::vector<EventCallBackFn> m_EventCallbacks;

		static Application* s_Instance;
	};

	Application* CreateApplication(int argc, char** argv);
}