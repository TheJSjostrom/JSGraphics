#pragma once
#include "glm/glm.hpp"
#include "Player.h"

namespace JSG {

	struct Perception
	{
		float DistanceToTarget = 0.0f;
		float AngleToTarget = 0.0f;
		float TargetWorldAngle = 0.0f;
	};

	enum class EnemyState : uint8_t
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
		const Perception& GetPerception() const { return m_Perception; }
		
		bool IsTargetInFOV() const;
		bool IsCloseToTarget(const Player& player) const;
	private:
		void UpdateAI(const Player& player);
		void UpdatePerception(const Player& player);
		void SetPerceptionData(const glm::vec3& displacement, float dotAngle);
		void DetermineEnemyState();
		void ExecuteStateAction(float ts, const Player& player);
		void HandleChase(float ts, const Player& player);

		void UpdateOrientation();
		void UpdateMovement(float ts);
		void UpdatePosition(float ts);
		void SetForwardDirection(const glm::vec3& direction);
		void UpdateColorPulse(float ts);
		void UpdateIdleState(float ts);
	private:
		EnemyState m_CurrentState = EnemyState::Idle;

		glm::vec3 m_Position = { -5.0f, 0.0f, 1.0f };
		glm::vec3 m_ForwardDirection = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_Color = { 1.0f, 0.0f, 0.0f };

		struct FieldOfView
		{
			float Range;
			float Angle;
		};

		FieldOfView m_Vision = { 10.0f, 45.0f };

		Perception m_Perception;

		float m_Speed = 4.0f;
		float m_Rotation = -90.0f;
		float m_Size = 1.0f;
		float m_PulseTimer = 0.0f;
	};
}