#include "editor.hpp"
#include "input.hpp"

namespace Engine {
	Editor::~Editor() {

	}

	void Editor::Run() {
		m_Window = GLWindow("Editor", 1280, 720, true);
		m_Camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

		m_Window.SetMouseCaptured(true);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		while (glfwWindowShouldClose(m_Window.GetWindowHandle())) {
			m_LogicLoop();
			m_RenderLoop();
		}
	}

	void Editor::m_LogicLoop() {
		m_UpdateDeltaTime();
	}

	void Editor::m_RenderLoop() {

	}

	void Editor::m_UpdateDeltaTime() {
		float currentFrame = static_cast<float>(glfwGetTime());
		m_DeltaTime = currentFrame - m_LastFrame;
		m_LastFrame = currentFrame;
	}

	void Editor::m_ProcessInput() {
		// Mouse.
		double xpos = 0.0;
		double ypos = 0.0;
		glfwGetCursorPos(m_Window.GetWindowHandle(), &xpos, &ypos);

		if (firstMouse) {
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		m_Camera.ProcessMouseMovement(xoffset, yoffset);

		// Keyboard.
		if (glfwGetKey(m_Window.GetWindowHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_Window.GetWindowHandle(), true);

		if (glfwGetKey(m_Window.GetWindowHandle(), GLFW_KEY_R) == GLFW_PRESS)
			m_Window.SetMouseCaptured(!m_Window.GetMouseCaptured());

		if (glfwGetKey(m_Window.GetWindowHandle(), GLFW_KEY_W) == GLFW_PRESS)
			m_Camera.ProcessKeyboard(FORWARD, m_DeltaTime);
		if (glfwGetKey(m_Window.GetWindowHandle(), GLFW_KEY_S) == GLFW_PRESS)
			m_Camera.ProcessKeyboard(BACKWARD, m_DeltaTime);
		if (glfwGetKey(m_Window.GetWindowHandle(), GLFW_KEY_A) == GLFW_PRESS)
			m_Camera.ProcessKeyboard(LEFT, m_DeltaTime);
		if (glfwGetKey(m_Window.GetWindowHandle(), GLFW_KEY_D) == GLFW_PRESS)
			m_Camera.ProcessKeyboard(RIGHT, m_DeltaTime);
	}
}
