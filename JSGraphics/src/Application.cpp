#include <iostream>

#include "Application.h"

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
			std::cout << "HELLO, WORLD!" << std::endl;
		}
	}

}