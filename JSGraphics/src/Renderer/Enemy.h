#pragma once
#include "glm/glm.hpp"
#include "Player.h"

namespace JSG {

	struct PerceptionData
	{
		float TargetDistance;         // The distance from the enemy to the player. displacementLength
		float TargetDirectionAngle;   // Angle the TargetDirection has - in degrees. displacementDirectionAngle
		float AngleToTargetDirection; // The angle to TargetDirection - in degrees. dotProductAngle
	};

	enum class EnemyState
	{ 
		Idle,
		Chase,
	};
	
	class Enemy
	{
	public:
		void OnUpdate(float ts, const Player& player);

		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetColor() const { return m_Color; }
		float GetRotation() const { return m_Rotation; }
		float GetSize() const { return m_Size; }
		const PerceptionData& GetPerceptionData() const { return m_PerceptionData; }
		
		bool IsTargetInFOV() const;
		bool IsCloseToTarget(const Player& player) const;
	private:
		void UpdatePerceptionData(const Player& player);
		void DetermineEnemyState();
		void HandleChase(float ts, const Player& player);
		void UpdateOrientation();
		void UpdateMovement(float ts);
		void UpdateForwardDirection();
		void UpdateColorPulse(float ts);
		void UpdateIdleState(float ts);
	private:
		EnemyState m_CurrentState = EnemyState::Idle;

		glm::vec3 m_Position = { -5.0f, 0.0f, 0.0f };
		glm::vec3 m_ForwardDirection = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_Color = { 1.0f, 0.0f, 0.0f };

		struct FOVData
		{
			const float Range;
			const float AngleDegrees;
		};

		FOVData m_FOVData = { 10.0f, 45.0f };

		PerceptionData m_PerceptionData;

		float m_Speed = 4.0f;
		float m_Rotation = -90.0f;
		float m_Size = 1.0f;
		float m_PulseTimer = 0.0f;
	};
}