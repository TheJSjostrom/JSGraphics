#pragma once
#include "glm/glm.hpp"

namespace JSG {

	enum class PlayerState
	{
		Idle,
		Walk
	};

	class Player
	{
	public:
		void OnUpdate(float ts);

		const glm::vec3& GetFarwardDirection() const { return m_ForwardDirection; }
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::vec3& GetColor() { return m_Color; }

		float GetRotation() const { return m_Rotation; }
		float GetSize() const { return m_Size; }
		float GetHitBox() const { return m_HitBox; }
	private:
		void UpdateIdleState(float ts);
		void UpdateWalkState(float ts);
	private:
		PlayerState m_CurrentState = PlayerState::Idle;

		glm::vec3 m_Position = { 11.0f, 5.0f, 0.0f };
		glm::vec3 m_Color = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_ForwardDirection = { 0.0f, 1.0f, 0.0f };

		float m_RotationVelocity = 180.0f;
		float m_Speed = 8.0f;
		float m_SizeVelocity = 1.0f;
		float m_Size = 1.0f;
		float m_Rotation = 90.0f;
		float m_HitBox = 1.25f;
		float m_RotationValue = 0.0f;
	};

}