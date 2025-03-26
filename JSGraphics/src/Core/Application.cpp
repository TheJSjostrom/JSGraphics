#include <iostream>

#include "GLFW/glfw3.h"
#include "Core/Application.h"


namespace JSG {

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window.OnUpdate();
		}
	}

}