#include "Enemy.h"
#include "Core/Input.h"

#include <GLFW/glfw3.h>

namespace JSG {

	namespace Utils
	{
		float CalculatedotAngle(const glm::vec3& v1, const glm::vec3& v2)
		{
			return glm::degrees(glm::acos(glm::clamp(glm::dot(v1, v2), -1.0f, 1.0f)));
		}

		Displacement CalculateDisplacement(const glm::vec3& v1, const glm::vec3& v2)
		{
			const glm::vec3 displacment = v1 - v2;
			return { glm::normalize(displacment), glm::length(displacment), glm::degrees(glm::atan(displacment.y, displacment.x)) };
		}
	}

	void Enemy::OnUpdate(float ts, const Player& player)
	{
		UpdatePerception(player);
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

	void Enemy::UpdatePerception(const Player& player)
	{
		const glm::vec3& playerPosition = player.GetPosition();
		const Displacement displacement = Utils::CalculateDisplacement(playerPosition, m_Position);

		const float dotAngle = Utils::CalculatedotAngle(m_ForwardDirection, displacement.Direction);

		m_Perception.DistanceToTarget = displacement.Length;
		m_Perception.TargetWorldAngle = displacement.Angle;
		m_Perception.AngleToTarget = dotAngle;
	};
	
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
		return m_Perception.AngleToTarget <= m_Vision.Angle &&
			   m_Perception.DistanceToTarget <= m_Vision.Range;
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
		return m_Perception.DistanceToTarget <= player.GetHitbox();
	}

	void Enemy::UpdateOrientation()
	{
		m_Rotation = m_Perception.TargetWorldAngle;
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