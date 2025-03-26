#include <iostream>
#include "Core/Window.h"

namespace JSG {

	Window::Window()
	{
		Init();
	}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::Init()
	{
		const bool IsglfwInit = (bool)glfwInit();
		if (!IsglfwInit)
		{
			std::cout << "Couldn't intialize glfw" << std::endl;
		}

		m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void Window::OnUpdate() const
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

}