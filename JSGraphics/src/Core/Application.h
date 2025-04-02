#pragma once
#include "Core/Window.h"
#include "Events/ApplicationEvent.h"

namespace JSG {

	class Application
	{
	public:
		Application();
		~Application();

		void OnEvent(Event& e);

		void Run();

	public:
		static Application* Get() { return s_Instance; }

	private:
		void OnUpdate();
		void OnRender();

		bool OnWindowClose(WindowCloseEvent& e);

	private:
		Window m_Window;

		bool m_Running = true;

	private:
		static Application* s_Instance;
	};

}