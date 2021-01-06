#pragma once

#include "Valhalla/Renderer/RendererContext.h"

struct GLFWwindow;

namespace Valhalla::Yggdrasil
{
	class OpenGLContext : public Heimdal::RendererContext
	{
	public:
		OpenGLContext(void* windowHandle);
		virtual ~OpenGLContext();

		virtual void Create() override;
		virtual void BeginFrame() override {}
		virtual void SwapBuffers() override;

		virtual void OnResize(uint32_t width, uint32_t height) override {}
	private:
		GLFWwindow* m_WindowHandle;
	};
}