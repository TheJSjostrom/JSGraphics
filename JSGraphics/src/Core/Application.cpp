#include "Core/Application.h"
#include "Core/Core.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include <iostream>
#include "glad/glad.h"

namespace JSG {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;
		m_Window.SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));
		m_ImGuiUI.Init();
	}

	Application::~Application()
	{
		m_ImGuiUI.Shutdown();
	}

	void Application::Run()
	{
		while (m_Running)
		{
		//	glClearColor(1.0f, 0.25f, 0.25f, 1.0f);
		//	glClear(GL_COLOR_BUFFER_BIT);

			m_Sandbox.OnUpdate();
			m_Sandbox.OnRender();

			m_ImGuiUI.Begin();
			m_Sandbox.OnImGuiRender();
			m_ImGuiUI.End();

			m_Window.OnUpdate();
		}
	}	

	void Application::OnEvent(Event& e)
	{
		std::cout << e << std::endl;

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));
	}

	bool Application::OnWindowClose(const WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}