#pragma once
#include "Core/Window.h"

namespace JSG {

	class Application
	{
	public:
		Application();
		~Application();

		void Run();

	private:
		Window m_Window;

		bool m_Running = true;
	};

}