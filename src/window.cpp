#include "window.hpp"
#include <bgfx/bgfx.h>
#include <iostream>

#if defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#elif defined(_WIN32) || defined(_WIN64)
#define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__APPLE__)
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>

namespace Engine {
	GLWindow::GLWindow(const std::string& title, unsigned int width, unsigned int height, bool VSync)
		:  m_Title(title), m_Width(width), m_Height(height), m_VSync(VSync), m_MouseCaptured(false) {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
		if (!m_Window)
			std::cerr << "Error: GLFW window creation failed." << std::endl;

		bgfx::Init bgfxInit;
#if defined(_WIN32)
		bgfxInit.platformData.nwh = glfwGetWin32Window(m_Window);
#elif defined(__APPLE__)
		bgfxInit.platformData.nwh = glfwGetCocoaWindow(m_Window);
#elif defined(__linux__)
		bgfxInit.platformData.ndt = glfwGetX11Display();
		bgfxInit.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(m_Window);
#endif
		bgfxInit.type = bgfx::RendererType::Count;
		bgfxInit.resolution.width = m_Width;
		bgfxInit.resolution.height = m_Height;
		bgfxInit.resolution.reset = m_VSync ? BGFX_RESET_VSYNC : BGFX_RESET_NONE;

		if (!bgfx::init(bgfxInit))
			std::cerr << "Failed to initialize bgfx!" << std::endl;
		
		bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x1A1A1AFF, 1.0f, 0);
		bgfx::setViewRect(0, 0, 0, uint16_t(m_Width), uint16_t(m_Height));

		glfwSetWindowUserPointer(m_Window, this);
		glfwSetFramebufferSizeCallback(m_Window, m_FramebufferSizeCallback);
	}

	GLWindow::~GLWindow() {
		glfwTerminate();
	}

	void GLWindow::SetVSync(bool flag) {
		m_VSync = flag;
		uint32_t resetFlags = m_VSync ? BGFX_RESET_VSYNC : BGFX_RESET_NONE;
		bgfx::reset(m_Width, m_Height, resetFlags);
	}

	void GLWindow::SetMouseCaptured(bool flag) {
		m_MouseCaptured = flag;
		if (!flag)
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void GLWindow::SetCursorPosCallback(GLFWcursorposfun callback) {
		glfwSetCursorPosCallback(m_Window, callback);
	}

	void GLWindow::SetScrollCallback(GLFWscrollfun callback) {
		glfwSetScrollCallback(m_Window, callback);
	}

	GLFWwindow* GLWindow::GetWindowHandle() {
		return m_Window;
	}

	void GLWindow::m_FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		GLWindow* myWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));
		if (myWindow) {
			myWindow->m_Width = width;
			myWindow->m_Height = height;

			uint32_t resetFlags = myWindow->m_VSync ? BGFX_RESET_VSYNC : BGFX_RESET_NONE;
			bgfx::reset(width, height, resetFlags);

			bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));
		}
	}
}
