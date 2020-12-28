#include "vhpch.h"
#include "OpenGLImGuiLayer.h"

#include <imgui.h>


#define IMGUI_IMPL_API
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "Valhalla/Core/Application.h"
#include <GLFW/glfw3.h>

#include "Valhalla/Renderer/Renderer.h"

namespace Valhalla
{
	namespace Yggdrasil
	{
		OpenGLImGuiLayer::OpenGLImGuiLayer()
		{
		}

		OpenGLImGuiLayer::OpenGLImGuiLayer(const std::string& name)
		{
		}

		OpenGLImGuiLayer::~OpenGLImGuiLayer()
		{
		}

		void OpenGLImGuiLayer::OnAttach()
		{
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// Enable Keyboard Control
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		// Enable Docking
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;	// Enable Multi-Viewports / Windows
			
			ImFont* pFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
			io.FontDefault = io.Fonts->Fonts.back();

			ImGui::StyleColorsDark();

			ImGuiStyle& style = ImGui::GetStyle();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				style.WindowRounding = 0.0f;
				style.Colors[ImGuiCol_WindowBg].w = 1.0f;
			}

			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, style.Colors[ImGuiCol_WindowBg].w);

			Application& app = Application::Get();
			GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 410");
		}

		void OpenGLImGuiLayer::OnDetach()
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		void OpenGLImGuiLayer::Begin()
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}

		void OpenGLImGuiLayer::End()
		{
			ImGuiIO& io = ImGui::GetIO();
			Application& app = Application::Get();
			io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}
		}

		void OpenGLImGuiLayer::OnImGuiRender()
		{

		}
	}
}