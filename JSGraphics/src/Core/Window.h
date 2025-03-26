#pragma once
#include <string>
#include <stdint.h>
#include <GLFW/glfw3.h>

namespace JSG {

	struct WindowProp
	{
		std::string Title = "JSGraphics Engine";
		uint32_t Width = 1280;
		uint32_t Height = 720;
	};

	class Window
	{
	public:
		Window();
		~Window();

		void OnUpdate() const;

		uint32_t GetWidth() const { return m_Data.Width; }
		uint32_t GetHeight() const { return m_Data.Width; }

	private:
		void Init();
		void Shutdown();

	private:
		GLFWwindow* m_Window;

		WindowProp m_Data;

	};

}