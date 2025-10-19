#pragma once
#include "window.hpp"

namespace Engine {
	class Editor {
	public:
		Editor();
		~Editor();

		void Run();
	private:
		GLWindow m_Window;

		void m_LogicLoop();
		void m_RenderLoop();
	};
}