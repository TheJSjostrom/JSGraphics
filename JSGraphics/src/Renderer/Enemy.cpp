#include "Enemy.h"
#include "Core/Input.h"

#include <GLFW/glfw3.h>

namespace JSG {

	void Enemy::OnUpdate(float ts, const Player& player)
	{
		UpdateForwardDirection();

		const auto perceptionData = DetermineEnemyState(player);

		switch (m_CurrentState)
		{
		case EnemyState::Idle:
			UpdateIdleState(ts); 
			break;
		case EnemyState::Chase:
			m_ChaseTime += ts;
			m_Speed = 4.0f;
			m_Color = { 0.8f, 0.8f, 0.8f };
			UpdateChaseState(ts, player.GetHitBox(), perceptionData);
			break;
		case EnemyState::Angry:
			UpdateAngryState(ts, player.GetHitBox(), perceptionData);
			UpdateColorPulse(ts);
			UpdateChaseState(ts, player.GetHitBox(), perceptionData);
			break;
		}
	}

	void Enemy::UpdateAngryState(float ts, float playerHitBox, const PerceptionData& perceptionData)
	{
		m_AngryChaseTime += ts;
		m_Speed = 10.0f;
	}

	void Enemy::UpdateForwardDirection()
	{
		m_ForwardDirection = { glm::cos(glm::radians(m_Rotation)), glm::sin(glm::radians(m_Rotation)), 0.0f };
		m_ForwardDirection = glm::normalize(m_ForwardDirection);
	}

	const PerceptionData Enemy::DetermineEnemyState(const Player& player)
	{
		const auto perceptionData = CalculatePerceptionData(player);
		// Check if player is in Field Of View.
		const bool isFOV = perceptionData.FOVAngleDegrees <= m_FOVAngle && perceptionData.DisplacementVectorLength <= m_FOVRange;
		// Angry Mode 
		bool isAngry = m_ChaseTime > 3.0f;

		if (m_AngryChaseTime > 2.0f)
		{
			isAngry = false;
			m_ChaseTime = 0.0f;
			m_AngryChaseTime = 0.0f;
		}

		if (isFOV && isAngry)
		{
			m_CurrentState = EnemyState::Angry;
		}
		else if (isFOV)
		{
			m_CurrentState = EnemyState::Chase;
		}
		else
		{
			m_CurrentState = EnemyState::Idle;
		}

		return perceptionData;
	}

	const PerceptionData Enemy::CalculatePerceptionData(const Player& player)
	{
		const glm::vec3& playerPosition = player.GetPosition();
		// Calculate the Displacement vector of Player Position and Enemy Position.
		const glm::vec3 displacementVector = playerPosition - m_Position;
		// Calculate the length of the DisplacementVectorLength vector.
		const float displacementVectorLength = glm::length(displacementVector);
		const glm::vec3 displacementVectorNormalized = glm::normalize(displacementVector);
		// Calculate the angle of displacementVectorNormalized
		const float displacementVectorNormalizedAngle = glm::degrees(glm::atan(displacementVectorNormalized.y, displacementVectorNormalized.x));
		// Calculate the Dot Product of m_EnemyForwardDirection and DisplacementVectorNormalized vectors.
		const float dotProduct = glm::dot(m_ForwardDirection, displacementVectorNormalized);
		// Calculate the angle - in degrees - between NormalizedDisplacementVector and m_EnemyForwardDirection vectors. dotProduct / (glm::length(displacementVectorNormalized) * glm::length(m_ForwardDirection)) = dotProduct
		const float FOVAngleDegrees = glm::degrees(glm::acos(glm::clamp(dotProduct, -1.0f, 1.0f)));

		return { displacementVectorLength, FOVAngleDegrees, displacementVectorNormalizedAngle };
	}

	void Enemy::UpdateChaseState(float ts, float playerHitBox, const PerceptionData& perceptionData)
	{
		// Checking if Enemy is touching player.
		const bool isTouching = perceptionData.DisplacementVectorLength <= playerHitBox;

		if (!isTouching)
		{
			UpdateOrientation(perceptionData);

			const glm::vec3 velocity = m_ForwardDirection * m_Speed * ts;
			m_Position += velocity;
		}
	}

	void Enemy::UpdateOrientation(const PerceptionData& perceptionData)
	{
		m_Rotation = perceptionData.TargetDirectionAngle;
		UpdateForwardDirection();
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