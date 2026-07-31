#include "Enemy.h"
#include "Core/Input.h"

#include <GLFW/glfw3.h>
#include <print>

namespace JSG {

	namespace Utils {

		static glm::vec3 CalculateVelocity(const glm::vec3& direction, float speed)
		{
			return direction * speed;
		}

		static float CalculatedotAngle(const glm::vec3& v1, const glm::vec3& v2)
		{
			return glm::degrees(glm::acos(glm::clamp(glm::dot(glm::normalize(v1), glm::normalize(v2)), -1.0f, 1.0f)));
		}

		static glm::vec3 CalculateDisplacement(const glm::vec3& start, const glm::vec3& end)
		{
			return end - start;
		}

		static float CalculateAngle(const glm::vec3& displacement)
		{
			return glm::degrees(glm::atan(displacement.y, displacement.x));
		}

		static glm::vec3 CalculateDirection(float rotation)
		{
			return glm::normalize(glm::vec3(glm::cos(glm::radians(rotation)), glm::sin(glm::radians(rotation)), 0.0f ));
		}

	}

	void Enemy::OnUpdate(float ts, const Player& player)
	{
		UpdateAI(player);
		ExecuteStateAction(ts, player);
	}

	void Enemy::UpdateAI(const Player& player)
	{
		UpdatePerception(player);
		DetermineEnemyState();
	}

	void Enemy::UpdatePerception(const Player& player)
	{
		const glm::vec3& playerPosition = player.GetPosition();
		const glm::vec3 displacement = Utils::CalculateDisplacement(m_Position, playerPosition);
		const float dotAngle = Utils::CalculatedotAngle(m_ForwardDirection, displacement);
		SetPerceptionData(displacement, dotAngle);
	}
	
	void Enemy::SetPerceptionData(const glm::vec3& displacement, float dotAngle)
	{
		m_Perception.DistanceToTarget = glm::length(displacement);
		m_Perception.TargetWorldAngle = Utils::CalculateAngle(displacement);
		m_Perception.AngleToTarget = dotAngle;
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

	void Enemy::ExecuteStateAction(float ts, const Player& player)
	{
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

		const glm::vec3 direction = Utils::CalculateDirection(m_Rotation);
		SetForwardDirection(direction);
	}

	void Enemy::SetForwardDirection(const glm::vec3& direction)
	{
		m_ForwardDirection = direction;
	}

	void Enemy::UpdateMovement(float ts)
	{
		UpdatePosition(ts);
	}
	
	void Enemy::UpdatePosition(float ts)
	{
		const glm::vec3 velocity = Utils::CalculateVelocity(m_ForwardDirection, m_Speed);
		m_Position += velocity * ts;
	}

	void Enemy::UpdateColorPulse(float ts)
	{
		const float speed = 4.0f;

		m_PulseTimer += speed * ts;
		const float colorIntensity = glm::abs(glm::cos(m_PulseTimer));
		m_Color = { colorIntensity, 0.0f, 0.0f };
	}

	void Enemy::UpdateIdleState(float ts)
	{
		m_Color = { 0.9f, 0.9f, 0.9f };
	}
}