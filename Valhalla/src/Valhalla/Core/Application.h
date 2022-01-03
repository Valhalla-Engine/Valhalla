#pragma once
#include "Valhalla/Core/Base.h"
#include "Valhalla/Core/TimeStep.h"
#include "Valhalla/Core/Timer.h"
#include "Valhalla/Core/Window.h"
#include "Valhalla/Core/LayerStack.h"

#include "Valhalla/Core/Events/ApplicationEvent.h"

namespace Valhalla
{

	struct ApplicationSpecification
	{
		std::string Name = "Valhalla";
		uint32_t WindowWidth = 1600, WindowHeight = 900;
		bool WindowDecorated = false;
		bool Fullscreen = false;
		bool VSync = true;
		std::string WorkingDirectory;
		bool StartMaximized = true;
		bool Resizeable = true;
		bool EnableImGui = true;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification);
		virtual ~Application();

		void Run();
		void Close();

		virtual void OnInit() {}
		virtual void OnShutdown();
		virtual void OnUpdate(Timestep ts) {}

		virtual void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::string OpenFile(const char* filter = "All\0*.*\0") const;
		std::string OpenFolder(const char* initialFolder = "") const;
		std::string SaveFile(const char* filter = "All\0*.*\0") const;

		void SetShowStats(bool show) { m_ShowStats = show; }

		inline Window& GetWindow() { return *m_Window; }

		static inline Application& Get() { return *s_Instance; }

		float_t GetTime() const;

		static const char* GetConfigurationName();
		static const char* GetPlatformName();

		const ApplicationSpecification& GetSpecification() const { return m_Specification; }

		PerformanceProfiler* GetPerformanceProfiler() { return m_Profiler; }

	private:
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClose(WindowCloseEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ApplicationSpecification m_Specification;
		bool m_Running = true, m_Minimized = false;
		LayerStack m_LayerStack;
		Timestep m_Timestep;
		PerformanceProfiler* m_Profiler = nullptr;
		bool m_ShowStats = true;

		float_t m_LastFrameTime = 0.0f;

		static Application* s_Instance;
	};

	Application* CreateApplication(int argc, char** argv);
}