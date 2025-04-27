#include "Core/Sandbox2D.h"
#include "Core/Window.h"
#include "Core/Application.h"

#include "imgui.h"
#include "glad/glad.h"

namespace JSG {

	Sandbox2D::Sandbox2D()
	{
	}

	Sandbox2D::~Sandbox2D()
	{
	}

	void Sandbox2D::OnUpdate()
	{
	}

	void Sandbox2D::OnRender()
	{
		glClearColor(1.0f, m_GValue, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Sandbox2D::OnImGuiRender()
	{
		ImGui::ShowDemoWindow();

		ImGui::Begin("Color");
		ImGui::SliderFloat("Background Color", &m_GValue, 0.0f, 1.0f);

		ImGui::Text("Hello, world %d", m_GValue);
		ImGui::End();
	}

	void Sandbox2D::OnEvent()
	{
	}

}
