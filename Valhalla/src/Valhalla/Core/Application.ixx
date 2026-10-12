module;
#include <queue>
#include <functional>
#include <mutex>

export module Valhalla.Core.Application;



import Valhalla.Core.Types;
import Valhalla.Core.Events;
import Valhalla.Core.TimeStep;
import Valhalla.Core.Window;
import Valhalla.Core.Layer;
import Valhalla.Core.LayerStack;
import Valhalla.Core.Timer;
import Valhalla.Core.Events.ApplicationEvent;
import Valhalla.Core.Window;

import Valhalla.Imgui.Layer;

export namespace Valhalla
{
	export struct ApplicationProps
	{
		std::string Name;
		uint32_t WindowWidth, WindowHeight;
	};

	export struct ApplicationSpecification
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

	export class Application
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