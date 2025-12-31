#include "Enemy.h"
#include "Core/Input.h"

#include <GLFW/glfw3.h>

namespace JSG {

	void Enemy::OnUpdate(float ts, const Player& player)
	{
		UpdatePerceptionData(player);
		DetermineEnemyState();

		switch (m_CurrentState)
		{
		case EnemyState::Idle:
			UpdateIdleState(ts); 
			break;
		case EnemyState::Chase:
			HandleChase(ts, player);
			UpdateColorPulse(ts);
			break;
		}
	}

	void Enemy::UpdatePerceptionData(const Player& player)
	{
		const glm::vec3& playerPosition = player.GetPosition();
		// Calculate the Displacement vector of Player Position and Enemy Position.
		const glm::vec3 displacement = playerPosition - m_Position;
		// Calculate the length of the displacement vector.
		const float displacementLength = glm::length(displacement);
		const glm::vec3 displacementDirection = glm::normalize(displacement);
		// Calculate the angle of displacementDirection
		const float displacementDirectionAngleDegrees = glm::degrees(glm::atan(displacementDirection.y, displacementDirection.x));
		// Calculate the Dot Product of m_ForwardDirection and displacementDirection vectors.
		const float dotProduct = glm::dot(m_ForwardDirection, displacementDirection);
		// Calculate the angle - in degrees - between displacementDirection and m_ForwardDirection vectors. dotProduct / (glm::length(displacementVectorNormalized) * glm::length(m_ForwardDirection)) = dotProduct
		const float dotProductAngleDegrees = glm::degrees(glm::acos(glm::clamp(dotProduct, -1.0f, 1.0f)));

		m_PerceptionData = { displacementLength, displacementDirectionAngleDegrees, dotProductAngleDegrees };
	}

	void Enemy::DetermineEnemyState()
	{
		if (IsTargetInFOV())
		{
			m_CurrentState = EnemyState::Chase;
		}
		else
		{
			m_CurrentState = EnemyState::Idle;
		}
	}

	bool Enemy::IsTargetInFOV() const
	{
		const bool isFOV = m_PerceptionData.AngleToTargetDirection <= m_FOVData.AngleDegrees &&
						   m_PerceptionData.TargetDistance <= m_FOVData.Range;

		return isFOV;
	}

	void Enemy::HandleChase(float ts, const Player& player)
	{
		if (!IsCloseToTarget(player))
		{
			UpdateOrientation();
			UpdateMovement(ts);
		}
	}

	bool Enemy::IsCloseToTarget(const Player& player) const
	{
		return m_PerceptionData.TargetDistance <= player.GetHitbox();
	}

	void Enemy::UpdateOrientation()
	{
		m_Rotation = m_PerceptionData.TargetDirectionAngle;
		UpdateForwardDirection();
	}

	void Enemy::UpdateMovement(float ts)
	{
		const glm::vec3 velocity = m_ForwardDirection * m_Speed * ts;
		m_Position += velocity;
	}

	void Enemy::UpdateForwardDirection()
	{
		const glm::vec3 forward = { glm::cos(glm::radians(m_Rotation)), glm::sin(glm::radians(m_Rotation)), 0.0f };
		m_ForwardDirection = glm::normalize(forward);
	}

	void Enemy::UpdateColorPulse(float ts)
	{
		m_PulseTimer += 4.0f * ts;
		const float colorIntensity = glm::abs(glm::cos(m_PulseTimer));
		m_Color = { colorIntensity, 0.0f, 0.0f };
	}

	void Enemy::UpdateIdleState(float ts)
	{
		m_Color = { 0.9f, 0.9f, 0.9f };
	}
}