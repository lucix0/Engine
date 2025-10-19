#include "window.hpp"
#include <iostream>

namespace Engine {
	GLWindow::GLWindow(const std::string& title, unsigned int width, unsigned int height, bool VSync)
		:  m_Title(title), m_Width(width), m_Height(height), m_VSync(VSync), m_MouseCaptured(false) {
		// Initialize GLFW and set window hints for OpenGL.
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
		if (!m_Window)
			std::cerr << "Error: GLFW window creation failed." << std::endl;

		glfwMakeContextCurrent(m_Window);

		// Set window callbacks.
		// These should be consistent between all windows.
		// Input related callback functions should be provided to the window manually.
		glfwSetErrorCallback(m_GLFWErrorCallback);
		glfwSetFramebufferSizeCallback(m_Window, m_FramebufferSizeCallback);

		// Load OpenGL functions.
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "Error: Failed to initialize GLAD." << std::endl;
		}

		SetVSync(m_VSync);

		glViewport(0, 0, m_Width, m_Height);

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(m_GLErrorCallback, 0);
	}

	GLWindow::~GLWindow() {
		glfwTerminate();
	}

	void GLWindow::SetVSync(bool flag) {
		m_VSync = flag;
		glfwSwapInterval(flag);
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

	void GLWindow::m_GLFWErrorCallback(int error, const char* description) {
		fprintf(stderr, "GLFW error %d: %s\n", error, description);
	}

	void GLWindow::m_FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}

	void GLAPIENTRY
		GLWindow::m_GLErrorCallback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
	{
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
	}
}


