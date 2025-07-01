#pragma once
#include <glfw/glfw3.h>

namespace Engine {
	class Editor {
	public:
		Editor();
		~Editor();

		void Run();
	private:
		GLFWwindow* m_Window;

		void m_CreateGLWindow();
		void m_RenderLoop();
	};
}