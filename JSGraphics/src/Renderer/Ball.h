#pragma once
#include "glm/glm.hpp"


namespace JSG {

	class Ball
	{
	public:
		Ball(const glm::vec3& position, const glm::vec3& color, float size);

		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetColor() const { return m_Color; }
		float GetSize() const { return m_Size; }

		void OnUpdate(float ts);
	private:
		glm::vec3 m_Position;
		glm::vec3 m_Color;
		float m_Size;

		glm::vec3 m_Velocity = { 0.0f, 1.0f, 0.0 };
		float m_Radius = 0.5f;
		float m_Gravity = -250.0f;
		float COR = 0.8f;
	};
}