#pragma once
#include "Core/Sandbox2D.h"
#include "Core/Window.h"

#include "Events/ApplicationEvent.h"

#include "ImGuiUI/ImGuiUI.h"
#include "Core/Sandbox.h"

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
		bool OnWindowResize(const WindowResizeEvent& e);
	private:
		Window m_Window;
		std::unique_ptr<Sandbox> m_Sandbox2D;

		float m_LastFrameTime = 0.0f;
		bool m_IsRunning = true;
	private:
		static Application* s_Instance;
	};

}