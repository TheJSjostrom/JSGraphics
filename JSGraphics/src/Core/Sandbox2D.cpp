#include "Core/Sandbox2D.h"
#include "Core/Window.h"
#include "Core/Application.h"
#include "Core/Core.h"
#include "Core/Input.h"

#include "Events/MouseEvent.h"

#include "imgui.h"

#include "glad/glad.h"

#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
namespace JSG {

	Sandbox2D::Sandbox2D()
	{
	}

	Sandbox2D::~Sandbox2D()
	{
	}

	void Sandbox2D::OnUpdate()
	{
		// Dot Product math
		glm::vec2 V = { m_XPos, m_YPos };
		glm::vec2 V2 = { -0.5f, 0.5f };

		m_DotProduct =  V.x * V2.x + V.y * V2.y;

		// Rotation math
		glm::vec2 iVector = { glm::cos(glm::radians(m_Angle)), glm::sin(glm::radians(m_Angle)) };
		m_IVector = iVector;
		glm::vec2 jVector = { -glm::sin(glm::radians(m_Angle)), glm::cos(glm::radians(m_Angle)) };
		glm::vec2 pos = { 0.5f, 0.5f };

		glm::vec2 transformedPos = pos.x * iVector + pos.y * jVector;
		const float transformedPosLength = glm::sqrt(transformedPos.x * transformedPos.x + transformedPos.y * transformedPos.y);
		glm::vec2 transformedPosUV = { transformedPos.x / transformedPosLength, transformedPos.y / transformedPosLength };
		m_PosUV = transformedPosUV;

		if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1) && Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		{
			m_Angle += 1.0f * (1/60.0f);
		}
		else if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2) && Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		{
			m_Angle -= 1.0f * (1 / 60.0f);
		}
		// x = .999848    y = .017452
	//	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_Angle), glm::vec3(0.0f, 0.0f, 1.0f));
		// 
		// [cos(45.0f) -sin(45.0f)]
		// [sin(45.0f)  cos(45.0f)]
		//std::cout << Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1) << std::endl;

	//  glm::vec4 position = { 0.5f, 0.5f, 0.0f, 1.0f};
		
	//	glm::vec4 transformedPosition = rotationMatrix * position;
		// position.x * [cos(45.0f)] + position.y * [-sin(45.0f)]
		//              [sin(45.0f)]                [ cos(45.0f)]
	}

	void Sandbox2D::OnRender()
	{
		glClearColor(1.0f, m_GValue, 0.25f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Sandbox2D::OnImGuiRender()
	{
		//ImGui::ShowDemoWindow();

		ImGui::Begin("Color");
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Color");
		ImGui::SliderFloat("Background Color", &m_GValue, 0.0f, 1.0f);
		ImGui::End();

		ImGui::Begin("Rotation");
		ImGui::SliderFloat("Change Angle", &m_Angle, 0.0f, 360.0f);
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "IVector");
		ImGui::Text("IVector X: %f", m_IVector.x);
		ImGui::Text("IVector Y: %f", m_IVector.y);
		ImGui::Text("IVector Angle: %f", m_Angle);

		ImGui::TextColored(ImVec4(1, 0, 0, 1), "transformedPos");
		ImGui::Text("TransformedPos VectorX: %f", m_PosUV.x);
		ImGui::Text("TransformedPos VectorY: %f", m_PosUV.y);
		ImGui::Text("IVector Angle: %f", m_Angle);
		ImGui::End();

		ImGui::Begin("Dot Product");
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Dot Product");
		ImGui::SliderFloat("Change X Position", &m_XPos, -5.0f, 5.0f);
		ImGui::SliderFloat("Change Y Position", &m_YPos, -5.0f, 5.0f);
		ImGui::Text("Dot Product: %f", m_DotProduct);
		ImGui::End();
	}

	void Sandbox2D::OnEvent(Event& e)
	{
	}

}
