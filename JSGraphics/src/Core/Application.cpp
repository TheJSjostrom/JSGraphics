#include "Core/Application.h"
#include "Core/Core.h"

#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/ApplicationEvent.h"

#include <iostream>
#include "glad/glad.h"

namespace JSG {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;
		m_Window.SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));
		ImGuiUI::Init();
	}

	Application::~Application()
	{
		ImGuiUI::Shutdown();
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Sandbox.OnUpdate();
			m_Sandbox.OnRender();

			ImGuiUI::Begin();
			m_Sandbox.OnImGuiRender();
			ImGuiUI::End();

			m_Window.OnUpdate();
		}
	}	

	void Application::OnEvent(Event& e)
	{
		std::cout << e.GetName() << std::endl;
	
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));

		m_Sandbox.OnEvent(e);
	}

	bool Application::OnWindowClose(const WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}