#include <iostream>

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
			std::cout << "ROBIN - PILLAR'N!" << std::endl;
		}
	}

}