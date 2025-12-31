#pragma once
#include "Events/Event.h"

#include <stdint.h>
#include <GLFW/glfw3.h>
#include <Functional>

namespace JSG {

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window();
		~Window();

		void OnUpdate();

		uint32_t GetWidth() const { return m_Data.Width; }
		uint32_t GetHeight() const { return m_Data.Height; }

		const std::string& GetTitle() const { return m_Data.Title; }

		void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled);

		bool IsVSync() const { return m_Data.VSync; }

		GLFWwindow* GetNativeWindow() const { return m_Window; }
	private:
		void Init();
		void Shutdown();

		void InitGLFW() const;
		void CreateGLFWWindow();
		void InitGLAD() const;
		void SetGLFWCallbacks() const;
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