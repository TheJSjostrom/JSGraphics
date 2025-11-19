#include "Ball.h"

namespace JSG {

	Ball::Ball(const glm::vec3& position, const glm::vec3& color, float size) :
		m_Position(position),
		m_Color(color),
		m_Size(size)
	{
	}

	void Ball::OnUpdate(float ts)
	{
		// 1. Apply gravitya
		m_Velocity.y += m_Gravity * ts;

		// 2. Update position
		m_Position.y += m_Velocity.y * ts;

		// 3. Check for collision with the ground
		if (m_Position.y + m_Radius <= 0.0f)
		{
			// Collision detected
			// a. Place the ball exactly on the ground to prevent "sinking" or jumping
			m_Position.y = 0.0f - m_Radius;

			// b. Reverse the vertical velocity and apply the CoR
			// This is the core "bounce" calculation
			m_Velocity.y = -m_Velocity.y * COR;
			
			// Optional: Apply ground friction to horizontal velocity
			// velocityX *= groundFriction; // e.g., 0.95
		}
	}
}