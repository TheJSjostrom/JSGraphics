#pragma once
#include "Core/Sandbox2D.h"
#include "Core/Window.h"
#include "Events/ApplicationEvent.h"
#include "ImGuiUI/ImGuiUI.h"

namespace JSG {

	class Application
	{
	public:
		Application();
		~Application();

		void Run();

		void OnEvent(Event& e);

		Window& GetWindow() { return m_Window; }
	public:
		static Application* Get() { return s_Instance; }
	private:
		bool OnWindowClose(const WindowCloseEvent& e);
	private:
		Window m_Window;
		ImGuiUI m_ImGuiUI;
		Sandbox2D m_Sandbox;

		bool m_Running = true;
	private:
		static Application* s_Instance;
	};

}