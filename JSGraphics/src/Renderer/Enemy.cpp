#include "Enemy.h"
#include "Core/Input.h"

#include <GLFW/glfw3.h>

namespace JSG {

	void Enemy::OnUpdate(float ts, const Player& player)
	{
		UpdateForwardDirection();
		
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

	void Enemy::UpdateForwardDirection()
	{
		m_ForwardDirection = { glm::cos(glm::radians(m_Rotation)), glm::sin(glm::radians(m_Rotation)), 0.0f };
		m_ForwardDirection = glm::normalize(m_ForwardDirection);
	}

	const EnemyFOVData Enemy::DetermineEnemyState(const Player& player)
	{
		const glm::vec3& playerPosition = player.GetPosition();
		// Calculate the Displacement vector of Player Position and Enemy Position.
		const glm::vec3 displacementVector = playerPosition - m_Position;
		// Calculate the length of the DisplacementVectorLength vector.
		const float displacementVectorLength = glm::length(displacementVector);
		// Normalize The Displacement vector.
		const glm::vec3 displacementVectorNormalized = glm::normalize(displacementVector);
		// Calculate the Dot Product of m_EnemyForwardDirection and DisplacementVectorNormalized vectors.
		const float dotProduct = glm::dot(m_ForwardDirection, displacementVectorNormalized);
		// Calculate the angle - in degrees - between NormalizedDisplacementVector and m_EnemyForwardDirection vectors.
		const float angleDegrees = glm::degrees(glm::acos(glm::clamp(dotProduct / (glm::length(displacementVectorNormalized) * glm::length(m_ForwardDirection)), -1.0f, 1.0f)));

		if (angleDegrees <= m_FOVAngle && displacementVectorLength <= m_FOVRange)
		{
			m_CurrentState = EnemyState::Chase;
		}
		else
		{
			m_CurrentState = EnemyState::Idle;
		}

		return { displacementVectorLength, displacementVectorNormalized };
	}

	void Enemy::UpdateChaseState(float ts, float playerHitBox, const EnemyFOVData& enemyFOVData)
	{
		if (enemyFOVData.DisplacementVectorLength >= playerHitBox)
		{
			const float displacementVectorNormalizedAngle = glm::degrees(glm::atan(enemyFOVData.DisplacementVectorNormalized.y, enemyFOVData.DisplacementVectorNormalized.x));
			m_Rotation = displacementVectorNormalizedAngle;
			m_ForwardDirection = enemyFOVData.DisplacementVectorNormalized;

			const glm::vec3 velocity = m_ForwardDirection * m_Speed * ts;
			m_Position += velocity;
		}

		UpdateColorPulse(ts);
	}

	void Enemy::UpdateColorPulse(float ts)
	{
		m_PulseTimer += 4.0f * ts;
		const float colorIntensity = glm::abs(glm::cos(m_PulseTimer));
		m_Color = { colorIntensity, 0.0f, 0.0f };
	}

	void Enemy::UpdateIdleState(float ts)
	{
		m_Color = { 0.8f, 0.8f, 0.8f };
	}
}