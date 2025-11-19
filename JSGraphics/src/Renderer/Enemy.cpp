#include "Enemy.h"
#include "Core/Input.h"

#include <GLFW/glfw3.h>

namespace JSG {

	void Enemy::OnUpdate(float ts, const Player& player)
	{
		m_ForwardDirection = { glm::cos(glm::radians(m_Rotation)), glm::sin(glm::radians(m_Rotation)), 0.0f };
		m_ForwardDirection = glm::normalize(m_ForwardDirection);

		if (Input::IsKeyPressed(GLFW_KEY_I))
		{
			m_Position.x += m_ForwardDirection.x * m_Velocity * ts;
			m_Position.y += m_ForwardDirection.y * m_Velocity * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_K))
		{
			m_Position.x -= m_ForwardDirection.x * m_Velocity * ts;
			m_Position.y -= m_ForwardDirection.y * m_Velocity * ts;
		}

		if (Input::IsKeyPressed(GLFW_KEY_J))
		{
			m_Rotation += 180.0f * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_L))
		{
			m_Rotation -= 180.0f * ts;
		}

		// Calculate the Displacement vector of Player Position and Enemy Position.
		const glm::vec3& playerPosition = player.GetPosition();
		const glm::vec3 DisplacementVector = playerPosition - m_Position;
		// Calculate the length of the DisplacementVectorLength vector.
		const float DisplacementVectorLength = glm::length(DisplacementVector);
		// Normalize The Displacement vector.
		const glm::vec3 DisplacementVectorNormalized = glm::normalize(DisplacementVector);
		// Calculate the Dot Product of m_EnemyForwardDirection and DisplacementVectorNormalized vectors.
		const float DotProduct = glm::dot(m_ForwardDirection, DisplacementVectorNormalized);
		// Calculate the angle between NormalizedDisplacementVector and m_EnemyForwardDirection vectors.
		const float angle = glm::degrees(glm::acos(glm::clamp(DotProduct / (glm::length(DisplacementVectorNormalized) * glm::length(m_ForwardDirection)), -1.0f, 1.0f)));
		
		if (angle <= m_FOVAngle && DisplacementVectorLength <= m_FOVRange)
		{
			m_CurrentState = EnemyState::Chase;
		}
		else
		{
			m_CurrentState = EnemyState::Idle;
		}

		switch (m_CurrentState) 
		{
			case EnemyState::Chase:
				UpdateChaseState(ts, player.GetHitBox(), DisplacementVectorLength, DisplacementVectorNormalized);
				break;
			case EnemyState::Idle:
				UpdateIdleState(ts);
				break;
		}
	}

	void Enemy::UpdateChaseState(float ts, float playerHitBox, float displacementVectorLength, const glm::vec3& displacementVectorNormalized)
	{
		if (displacementVectorLength >= playerHitBox)
		{
			m_Rotation = glm::degrees(glm::atan(m_ForwardDirection.y, m_ForwardDirection.x));
			const float DisplacementVectorNormalizedAngle = glm::degrees(glm::atan(displacementVectorNormalized.y, displacementVectorNormalized.x));
			m_Rotation = DisplacementVectorNormalizedAngle;
			m_ForwardDirection = displacementVectorNormalized;

			m_Position.x += m_ForwardDirection.x * m_Velocity * ts;
			m_Position.y += m_ForwardDirection.y * m_Velocity * ts;
		}

		m_RotationValue += 4.0f * ts;
		float CosValue = glm::abs(glm::cos(m_RotationValue));
		m_Color = { CosValue, 0.0f, 0.0f };
	}

	void Enemy::UpdateIdleState(float ts)
	{
		m_Color = { 0.0f, 0.5f, 0.5f };
	}
}