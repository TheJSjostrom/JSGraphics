#pragma once
#include "glm/glm.hpp"

namespace JSG {

	class Circle
	{
	public:
		Circle(float size, const glm::vec3& position, const glm::vec3& color);

		float GetSize() const { return m_Size; }
		const glm::vec3& GetColor() const { return m_Color; }
		const glm::vec3& GetPosition() const { return m_Position; }
	private:
		float m_Size;
		glm::vec3 m_Position;
		glm::vec3 m_Color;
	};

}