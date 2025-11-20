#pragma once
#include "glm/glm.hpp"
#include "Player.h"

namespace JSG {

	struct EnemyFOVData
	{
		float AngleDegrees;
		float DisplacementVectorLength;
		glm::vec3 DisplacementVectorNormalized;
	};

	enum class EnemyState 
	{ 
		Idle, 
		Chase
	};

	class Enemy
	{
	public:
		void OnUpdate(float ts, const Player& player);

		const glm::vec3& GetPosition() const { return m_Position; }
		float GetRotation() const { return m_Rotation; }
		float GetSize() const { return m_Size; }
		const glm::vec3& GetColor() const { return m_Color; }
	private:
		void HandleRotation(float ts);
		void UpdateForwardDirection();
		void HandleMovement(float ts);
		EnemyFOVData DetermineEnemyState(const Player& player);
		void UpdateChaseState(float ts, float playerHitBox, const EnemyFOVData& enemyFOVData);
		void UpdateIdleState(float ts);
	private:
		EnemyState m_CurrentState = EnemyState::Idle;
		EnemyFOVData m_FOVData;

		glm::vec3 m_Position = { -5.0f, 0.0f, 0.0f };
		glm::vec3 m_ForwardDirection = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_Color = { 1.0f, 0.0f, 0.0f };

		float m_Speed = 4.0f;
		float m_FOVRange = 10.0f;
		float m_FOVAngle = 45.0f;
		float m_Rotation = -90.0f;
		float m_RotationSpeed = 4.0f;
		float m_Size = 1.0f;
		const float FACING_THRESHOLD = 0.996f;
		float m_RotationValue = 0.0f;

	};

}