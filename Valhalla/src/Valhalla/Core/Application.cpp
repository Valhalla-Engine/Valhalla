#include "vhpch.h"
#include "Application.h"

#include "Input.h"

#include <filesystem>

extern bool g_ApplicationRunning;

namespace Valhalla
{
#define BIND_EVENT_FN(fn) std::bind(&Application::##fn, this , std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		s_Instance = this;

		if (!specification.WorkingDirectory.empty())
			std::filesystem::current_path(specification.WorkingDirectory);

		m_Profiler = new PerformanceProfiler();


	}

	Application::~Application()
	{
		for (Layer* layer : m_LayerStack)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack.PopLayer(layer);
		layer->OnDetach();
	}

	void Application::PopOverlay(Layer* overlay)
	{
		m_LayerStack.PopOverlay(overlay);
		overlay->OnDetach();
	}

	void Application::Run()
	{
		while (true);
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnShutdown()
	{
		g_ApplicationRunning = false;
	}

	void Application::OnEvent(Event& event)
	{
		EventDispathcer dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) {return OnWindowResize(e); });
		dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) {return OnWindowClose(e); });

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(event);
			if (event.Handled)
				break;
		}
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		const uint32_t width = e.GetWidth(), height = e.GetHeight();
		if (width == 0 || height == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		Close();
		return false;
	}

	std::string Application::OpenFile(const char* filter) const
	{
		return std::string();
	}

	std::string Application::OpenFolder(const char* initialFolder) const
	{
		return std::string();
	}

	std::string Application::SaveFile(const char* filter) const
	{
		return std::string();
	}

	float_t Application::GetTime() const
	{
		return 0.0f;
	}

	const char* Application::GetConfigurationName()
	{
		return "";
	}

	const char* Application::GetPlatformName()
	{
		return "Windows x64";
	}
}