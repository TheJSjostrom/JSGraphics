#pragma once
#include <string>
#include <stdint.h>
#include <GLFW/glfw3.h>
#include <Functional>
#include "Events/Event.h"

namespace JSG {

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window();
		~Window();

		void OnUpdate();

		uint32_t GetWidth() const { return m_Data.Width; }
		uint32_t GetHeight() const { return m_Data.Width; }

		void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled);
		bool IsVSync() { return m_Data.VSync; }

	private:
		void Init();
		void Shutdown();

	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title = "JSGraphics";
			uint32_t Width = 1280;
			uint32_t Height = 720;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}