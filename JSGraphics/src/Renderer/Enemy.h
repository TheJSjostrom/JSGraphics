#pragma once
#include "glm/glm.hpp"
#include "Player.h"

namespace JSG {

	struct PerceptionData
	{
		float DisplacementVectorLength;
		float FOVAngleDegrees;
		float TargetDirectionAngle;

		PerceptionData(float length, float FOVAngle, float targetDirectionAngle) :
			DisplacementVectorLength(length),
			FOVAngleDegrees(FOVAngle),
			TargetDirectionAngle(targetDirectionAngle)
		{}
	};

	enum class EnemyState
	{ 
		Idle,
		Chase,
		Angry
	};
	
	class Enemy
	{
	public:
		void OnUpdate(float ts, const Player& player);

		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetColor() const { return m_Color; }
		float GetRotation() const { return m_Rotation; }
		float GetSize() const { return m_Size; }
	private:
		void UpdateForwardDirection();
		const PerceptionData DetermineEnemyState(const Player& player);
		const PerceptionData CalculatePerceptionData(const Player& player);
		void UpdateAngryState(float ts, float playerHitBox, const PerceptionData& perceptionData);
		void UpdateChaseState(float ts, float playerHitBox, const PerceptionData& perceptionData);
		void UpdateOrientation(const PerceptionData& perceptionData);
		void UpdateColorPulse(float ts);
		void UpdateIdleState(float ts);
	private:
		EnemyState m_CurrentState = EnemyState::Idle;

		glm::vec3 m_Position = { -5.0f, 0.0f, 0.0f };
		glm::vec3 m_ForwardDirection = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_Color = { 1.0f, 0.0f, 0.0f };

		float m_Speed = 4.0f;
		float m_FOVRange = 20.0f;
		float m_FOVAngle = 45.0f;
		float m_Rotation = -90.0f;
		float m_Size = 1.0f;
		float m_PulseTimer = 0.0f;

		float m_ChaseTime = 0.0f;
		float m_AngryChaseTime = 0.0f;
	};
}