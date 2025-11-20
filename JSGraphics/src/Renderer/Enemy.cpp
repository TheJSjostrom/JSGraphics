#include "Enemy.h"
#include "Core/Input.h"

#include <GLFW/glfw3.h>

namespace JSG {

	void Enemy::OnUpdate(float ts, const Player& player)
	{
		HandleRotation(ts);
		UpdateForwardDirection();
		HandleMovement(ts);
		const EnemyFOVData enemyFOVData = DetermineEnemyState(player);

		switch (m_CurrentState) 
		{
		case EnemyState::Idle:
			UpdateIdleState(ts);
			break;
		case EnemyState::Chase:
			UpdateChaseState(ts, player.GetHitBox(), enemyFOVData);
			break;
		}
	}

	void Enemy::HandleRotation(float ts)
	{
		if (Input::IsKeyPressed(GLFW_KEY_J))
		{
			m_Rotation += 180.0f * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_L))
		{
			m_Rotation -= 180.0f * ts;
		}
	}

	void Enemy::UpdateForwardDirection()
	{
		m_ForwardDirection = { glm::cos(glm::radians(m_Rotation)), glm::sin(glm::radians(m_Rotation)), 0.0f };
		m_ForwardDirection = glm::normalize(m_ForwardDirection);
	}

	void Enemy::HandleMovement(float ts)
	{
		if (Input::IsKeyPressed(GLFW_KEY_I))
		{
			m_Position.x += m_ForwardDirection.x * m_Speed * ts;
			m_Position.y += m_ForwardDirection.y * m_Speed * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_K))
		{
			m_Position.x -= m_ForwardDirection.x * m_Speed * ts;
			m_Position.y -= m_ForwardDirection.y * m_Speed * ts;
		}
	}

	EnemyFOVData Enemy::DetermineEnemyState(const Player& player)
	{
		EnemyFOVData enemyFOVData;

		const glm::vec3& playerPosition = player.GetPosition();
		// Calculate the Displacement vector of Player Position and Enemy Position.
		const glm::vec3 DisplacementVector = playerPosition - m_Position;
		// Calculate the length of the DisplacementVectorLength vector.
		enemyFOVData.DisplacementVectorLength = glm::length(DisplacementVector);
		// Normalize The Displacement vector.
		enemyFOVData.DisplacementVectorNormalized = glm::normalize(DisplacementVector);
		// Calculate the Dot Product of m_EnemyForwardDirection and DisplacementVectorNormalized vectors.
		const float DotProduct = glm::dot(m_ForwardDirection, enemyFOVData.DisplacementVectorNormalized);
		// Calculate the angle - in degrees - between NormalizedDisplacementVector and m_EnemyForwardDirection vectors.
		enemyFOVData.AngleDegrees = glm::degrees(glm::acos(glm::clamp(DotProduct / (glm::length(enemyFOVData.DisplacementVectorNormalized) * glm::length(m_ForwardDirection)), -1.0f, 1.0f)));

		if (enemyFOVData.AngleDegrees <= m_FOVAngle && enemyFOVData.DisplacementVectorLength <= m_FOVRange)
		{
			m_CurrentState = EnemyState::Chase;
		}
		else
		{
			m_CurrentState = EnemyState::Idle;
		}

		return enemyFOVData;
	}

	void Enemy::UpdateChaseState(float ts, float playerHitBox, const EnemyFOVData& enemyFOVData)
	{
		if (enemyFOVData.DisplacementVectorLength >= playerHitBox)
		{
			m_Rotation = glm::degrees(glm::atan(m_ForwardDirection.y, m_ForwardDirection.x));
			const float DisplacementVectorNormalizedAngle = glm::degrees(glm::atan(enemyFOVData.DisplacementVectorNormalized.y, enemyFOVData.DisplacementVectorNormalized.x));
			m_Rotation = DisplacementVectorNormalizedAngle;
			m_ForwardDirection = enemyFOVData.DisplacementVectorNormalized;

			m_Position.x += m_ForwardDirection.x * m_Speed * ts;
			m_Position.y += m_ForwardDirection.y * m_Speed * ts;
		}

		m_RotationValue += 4.0f * ts;
		float CosValue = glm::abs(glm::cos(m_RotationValue));
		m_Color = { CosValue, 0.0f, 0.0f };
	}

	void Enemy::UpdateIdleState(float ts)
	{
		m_Color = { 0.8f, 0.8f, 0.8f };
	}
}