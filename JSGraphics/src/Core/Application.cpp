#include "Core/Application.h"
#include "Core/Core.h"

#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/ApplicationEvent.h"

#include <iostream>
#include "glad/glad.h"
#include <memory>

namespace JSG {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;
		m_Window.SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));
		ImGuiUI::Init();
		m_Sandbox2D = std::make_unique<Sandbox2D>();
	}

	Application::~Application()
	{
		ImGuiUI::Shutdown();
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Sandbox2D->OnUpdate();
			m_Sandbox2D->OnRender();

			ImGuiUI::Begin();
			m_Sandbox2D->OnImGuiRender();
			ImGuiUI::End();

			m_Window.OnUpdate();
		}
	}	

	void Application::OnEvent(Event& e)
	{
		std::cout << e.GetName() << std::endl;
 
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Application::OnWindowResize));

		m_Sandbox2D->OnEvent(e);
	}

	bool Application::OnWindowClose(const WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(const WindowResizeEvent& e)
	{
		glViewport(0, 0, e.GetWidth(), e.GetHeight());
		return true;
	}

}