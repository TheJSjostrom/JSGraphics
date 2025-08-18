#include "Renderer/Circle.h"

namespace JSG {

	Circle::Circle(float size, const glm::vec3& position, const glm::vec3& color) :
		m_Size(size),
		m_Position(position),
		m_Color(color)
	{
	}

}