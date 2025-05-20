#include "Core/Sandbox2D.h"
#include "Core/Window.h"
#include "Core/Application.h"
#include "Core/Core.h"

#include "Events/MouseEvent.h"

#include "imgui.h"

#include "glad/glad.h"

#include "glm/gtc/matrix_transform.hpp"

namespace JSG {

	Sandbox2D::Sandbox2D()
	{
	}

	Sandbox2D::~Sandbox2D()
	{
	}

	void Sandbox2D::OnUpdate()
	{
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_Angle), glm::vec3(0.0f, 0.0f, 1.0f));
		// 
		// [cos(45.0f) -sin(45.0f)]
		// [sin(45.0f)  cos(45.0f)]
		const float angle = 45.0f;
		glm::vec2 iVector = { glm::cos(glm::radians(angle)), glm::sin(glm::radians(angle)) };
		glm::vec2 jVector = { -glm::sin(glm::radians(angle)), glm::cos(glm::radians(angle)) };
		glm::vec2 pos = { glm::cos(glm::radians(45.0f)),  glm::sin(glm::radians(45.0f)) };
		glm::vec2 transformedPos = pos.x * iVector + pos.y * jVector;
	
		glm::vec4 position = { 0.5f, 0.5f, 0.0f, 1.0f};
		
		float positionLength = glm::sqrt(position.x * position.x + position.y * position.y);
		m_Length = positionLength;
		glm::vec4 transformedPosition = rotationMatrix * position;
		m_Pos = transformedPosition;
		// position.x * [cos(45.0f)] + position.y * [-sin(45.0f)]
		//              [sin(45.0f)]                [ cos(45.0f)]
		float transformedPositionLength = glm::sqrt(transformedPosition.x * transformedPosition.x + transformedPosition.y * transformedPosition.y);
		m_LengthTrans = transformedPositionLength;
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
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Color");
		ImGui::SliderFloat("Background Color", &m_GValue, 0.0f, 1.0f);
		ImGui::SliderFloat("Change angle", &m_Angle, 0.0f, 360.0f);

		ImGui::Text("X: %f", m_Pos.x);
		ImGui::Text("Y: %f", m_Pos.y);
		ImGui::Text("Angle: %f", m_Angle);
		ImGui::Text("Length: %f", m_Length);
		ImGui::Text("LengthTrans: %f", m_LengthTrans);
		ImGui::End();
	}

	void Sandbox2D::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(Sandbox2D::OnMouseButtonPressed));
	}

	bool Sandbox2D::OnMouseButtonPressed(const MouseButtonPressedEvent& e)
	{
		m_Angle += 1.0f;

		return false;
	}

}
