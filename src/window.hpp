#pragma once
#include <string>
#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace Engine {
	class GLWindow {
	public:
		GLWindow(const std::string& title, unsigned int width, unsigned int height, bool VSync);
		~GLWindow();

		void SetVSync(bool flag);
		void SetMouseCaptured(bool flag);

		bool GetVSync();
		bool GetMouseCaptured();

		void SetCursorPosCallback(GLFWcursorposfun callback);
		void SetScrollCallback(GLFWscrollfun callback);

		GLFWwindow* GetWindowHandle();
	private:
		GLFWwindow* m_Window;
		std::string m_Title;
		unsigned int m_Width;
		unsigned int m_Height;

		bool m_VSync;
		bool m_MouseCaptured;

		static void m_GLFWErrorCallback(int error, const char* description);
		static void m_FramebufferSizeCallback(GLFWwindow* window, int width, int height);
		static void GLAPIENTRY m_GLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	};
}