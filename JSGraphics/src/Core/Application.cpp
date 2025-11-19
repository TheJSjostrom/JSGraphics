#include "Core/Application.h"
#include "Core/Core.h"

#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/ApplicationEvent.h"

#include <print>
#include <iostream>
#include <memory>

#include "glad/glad.h"

namespace JSG {

	Application* Application::s_Instance = nullptr;

	Application::Application() : 
		m_Sandbox2D(std::make_unique<Sandbox2D>())
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
		// Application Loop
		while (m_Running)
		{
			Sandbox2D&& sand = Sandbox2D();
	
			float time = static_cast<float>(glfwGetTime());
			float timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			m_Sandbox2D->OnUpdate(timestep);
			m_Sandbox2D->OnRender();

			ImGuiUI::Begin();
			m_Sandbox2D->OnImGuiRender();
			ImGuiUI::End();

			m_Window.OnUpdate();
		}
	}	

	void Application::OnEvent(Event& e)
	{
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