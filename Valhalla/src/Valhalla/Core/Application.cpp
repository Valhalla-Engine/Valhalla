#include "vhpch.h"
#include "Application.h"

#include "Valhalla/Renderer/Renderer.h"


//extern ImGuiContext* GImGui;
namespace Valhalla
{
#define BIND_EVENT_FN(fn) std::bind(&Application::##fn, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specs)
		: m_Specification(specs)
	{
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(m_Specification.Name, 
			m_Specification.WindowWidth, m_Specification.WindowHeight)));
		m_Window->Init();
		m_Window->SetEventCallback([this](Event& e) {OnEvent(e); });
		//m_Window->Maximize();
		m_Window->SetVSync(true);

		m_Profiler = new PerformanceProfiler();

		Heimdall::Renderer::Init();
		Heimdall::Renderer::WaitAndRender();

		//m_ImGuiLayer = ImGuiLayer::Create();
		//PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		for (Layer* layer : m_LayerStack)
			layer->OnDetach();

		Heimdall::Renderer::Shutdown();

		delete m_Profiler;
		m_Profiler = nullptr;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack.PopLayer(layer);
		layer->OnDetach();
	}

	void Application::PopOverlay(Layer* layer)
	{
		m_LayerStack.PopOverlay(layer);
		layer->OnDetach();
	}

	void Application::RenderImGui()
	{
		m_ImGuiLayer->Begin();

		for (uint32_t i = 0; i < m_LayerStack.Size(); i++)
			m_LayerStack[i]->OnImGuiRender();
	}

	void Application::Run()
	{
		OnInit();
		while (m_Running)
		{
			if (!m_Minimized)
			{
				Heimdall::Renderer::BeginFrame();

				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(m_TimeStep);

				//RenderImGui();
				//m_ImGuiLayer->End();
				Heimdall::Renderer::WaitAndRender();
				Heimdall::Renderer::EndFrame();
			}

			ProcessEvents();
		}
		OnShutdown();
	}

	void Application::ProcessEvents()
	{
		m_Window->ProcessEvents();

		std::scoped_lock<std::mutex> lock(m_EventQueueMutex);

		while (m_EventQueue.size() > 0)
		{
			auto& func = m_EventQueue.front();
			func();
			m_EventQueue.pop();
		}
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return OnWindowResize(e); });
		dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return OnWindowClose(e); });

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(event);
			if (event.Handled)
				break;
		}

		if (event.Handled)
			return;

		for (auto& eventCallback : m_EventCallbacks)
		{
			eventCallback(event);

			if (event.Handled)
				break;
		}
	}

	bool_t Application::OnWindowResize(WindowResizeEvent& e)
	{
		int32_t width = e.GetWidth(), height = e.GetHeight();
		if (width == 0 || height == 0)
		{
			return false;
		}

		auto& window = m_Window;


		return false;
	}

	bool_t Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}