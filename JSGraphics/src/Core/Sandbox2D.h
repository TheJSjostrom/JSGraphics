#pragma once
#include "Events/Event.h"
#include "Events/MouseEvent.h"

#include "glm/glm.hpp"

namespace JSG {

	class Sandbox2D
	{
	public:
		Sandbox2D();
		~Sandbox2D();

		void OnUpdate();
		void OnRender();
		void OnImGuiRender();

		void OnEvent(Event& e);
	private:
		bool OnMouseButtonPressed(const MouseButtonPressedEvent& e);
	private:
		float m_GValue = 0.25f;
		float m_Angle = 0.0f;
		float m_Length = 0.0f;
		float m_LengthTrans = 0.0f;

		glm::vec4 m_Pos = { 0.0f, 0.0f, 0.0f, 0.0f };
	};

}