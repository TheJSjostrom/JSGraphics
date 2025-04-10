#include <iostream>

#include "glad/glad.h"
#include "Core/Application.h"
#include "Core/Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

namespace JSG {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;
		m_Window.SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));
		m_ImGuiUI = std::make_unique<ImGuiUI>();

	/*
		auto bindFn = HZ_BIND_EVENT_FN(Application::OnEvent);
		WindowCloseEvent e;
		std::function<void(Event&)> x = (std::function<void(Event&)>)bindFn;
		x(e);
		std::string name = std::string("apa");
	*/

	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			OnRender();
			OnUpdate();

			m_Window.OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		std::cout << e << std::endl;

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));

		m_ImGuiUI->OnEvent(e);
	}

	void Application::OnUpdate()
	{
	}

	void Application::OnRender()
	{
		glClearColor(1.0f, 0.25f, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_ImGuiUI->OnRender();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}