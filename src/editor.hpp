#pragma once
#include "window.hpp"
#include "camera.hpp"

namespace Engine {
	class Editor {
	public:
		Editor();
		~Editor();

		void Run();
	private:
		GLWindow m_Window;
		Camera m_Camera;

		float m_DeltaTime;
		float m_LastFrame;

		void m_LogicLoop();
		void m_RenderLoop();

		void m_UpdateDeltaTime();
		void m_ProcessInput();
	};
}