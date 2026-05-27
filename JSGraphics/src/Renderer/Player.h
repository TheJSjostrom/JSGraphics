#pragma once
#include "glm/glm.hpp"

namespace JSG {

	enum class PlayerState
	{
		Idle,
		Walk,
		Sprint
	};

	class Player
	{
	public:
		void OnUpdate(float ts);

		void UpdateColorPulse(float ts, uint32_t color);

		const glm::vec3& GetForwardDirection() const { return m_ForwardDirection; }

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; }

		glm::vec3& GetColor() { return m_Color; }
		float GetRotation() const { return m_Rotation; }
		float GetSize() const { return m_Size; }
		float GetHitbox() const { return m_HitBox; }

		bool IsOutOfBound() const;
	private:
		bool IsWalking() const;
		bool IsSprinting() const;
		void HandleRotation(float ts);
		void UpdateForwardDirection();
		void DeterminePlayerState();
		void UpdateIdleState(float ts);
		void UpdateWalkState(float ts);
		void UpdateRunState(float ts);
		glm::vec3 CalculateVelocity(float ts) const;
		void HandleMovement(float ts);
	private:
		PlayerState m_CurrentState = PlayerState::Idle;

		glm::vec3 m_Position = { 11.0f, 2.0f, 1.0f };
		glm::vec3 m_Color = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_ForwardDirection = { 0.0f, 0.0f, 0.0f };

		float m_RotationSpeed = 180.0f;
		float m_Speed = 4.0f;
		float m_Size = 1.0f;
		float m_Rotation = -90.0f;
		float m_HitBox = 1.25f;
		float m_PulseTimer = 0.0f;
		float m_PulseSpeed = 0.0f;
	};

}