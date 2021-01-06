#include "vhpch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Valhalla::Yggdrasil
{
	OpenGLContext::OpenGLContext(void* windowHandle)
	{
		m_WindowHandle = static_cast<GLFWwindow*>(windowHandle);
	}

	OpenGLContext::~OpenGLContext()
	{
	}

	void OpenGLContext::Create()
	{
		VH_CORE_INFO("OpenGLContext::Create");

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		VH_CORE_ASSERT(status, "Failed to initialize Glad!");
		
		VH_CORE_INFO("OpenGl info:");
		VH_CORE_INFO("	Vendor: {0}", glGetString(GL_VENDOR));
		VH_CORE_INFO("	Renderer: {0}", glGetString(GL_RENDERER));
		VH_CORE_INFO("	Version: {0}", glGetString(GL_VERSION));

#ifdef VH_ENABLE_ASSERTS
		int32_t versionMajor;
		int32_t versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		VH_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Valhalla requires at least OpenGL version 4.5!");
#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}		