#include "Player.h"
#include "GLFW/glfw3.h"
#include "Core/Input.h"

namespace JSG {

	void Player::OnUpdate(float ts)
	{
		m_ForwardDirection = { glm::cos(glm::radians(m_Rotation)), glm::sin(glm::radians(m_Rotation)), 0.0f };
		m_ForwardDirection = glm::normalize(m_ForwardDirection);

		const bool isWalking = Input::IsKeyPressed(GLFW_KEY_UP) || Input::IsKeyPressed(GLFW_KEY_DOWN);
		if (isWalking)
		{
			m_CurrentState = PlayerState::Walk;
		}
		else
		{
			m_CurrentState = PlayerState::Idle;
		}
	
		switch (m_CurrentState)
		{
		case PlayerState::Idle:
			UpdateIdleState(ts);
			break;
		case PlayerState::Walk:
			UpdateWalkState(ts);
			break;
		}

		// PLAYER ROTATION
		if (Input::IsKeyPressed(GLFW_KEY_LEFT))
		{
			m_Rotation += m_RotationVelocity * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_RIGHT))
		{
			m_Rotation -= m_RotationVelocity * ts;
		}

		// PLAYER SIZE
		if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		{
			m_Size += m_SizeVelocity * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_X))
		{
			m_Size -= m_SizeVelocity * ts;
		}

		m_Size = std::max(m_Size, 0.5f);

	}

	void Player::UpdateIdleState(float ts)
	{
		m_Color = { 0.0f, 1.0f, 0.0f };
	}

	void Player::UpdateWalkState(float ts)
	{
		m_RotationValue += 4.0f * ts;
		float CosValue = glm::abs(glm::cos(m_RotationValue));
		m_Color = { 0.0f, CosValue, 0.0f };

		if (Input::IsKeyPressed(GLFW_KEY_UP))
		{
			m_Position.x += m_ForwardDirection.x * m_Speed * ts;
			m_Position.y += m_ForwardDirection.y * m_Speed * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_DOWN))
		{
			m_Position.x -= m_ForwardDirection.x * m_Speed * ts;
			m_Position.y -= m_ForwardDirection.y * m_Speed * ts;
		}

	}

}