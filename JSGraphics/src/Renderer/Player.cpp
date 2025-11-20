#include "Player.h"
#include "GLFW/glfw3.h"
#include "Core/Input.h"

namespace JSG {

	void Player::OnUpdate(float ts)
	{
		HandleRotation(ts);
		UpdateForwardDirection();
		DeterminePlayerState();
	
		switch (m_CurrentState)
		{
		case PlayerState::Idle:
			UpdateIdleState(ts);
			break;
		case PlayerState::Walk:
			UpdateWalkState(ts);
			break;
		case PlayerState::Run:
			UpdateRunState(ts);
			break;
		}
	}

	void Player::HandleRotation(float ts)
	{
		if (Input::IsKeyPressed(GLFW_KEY_LEFT))
		{
			m_Rotation += m_RotationSpeed * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_RIGHT))
		{
			m_Rotation -= m_RotationSpeed * ts;
		}
	}

	void Player::UpdateForwardDirection()
	{
		m_ForwardDirection = { glm::cos(glm::radians(m_Rotation)), glm::sin(glm::radians(m_Rotation)), 0.0f };
		m_ForwardDirection = glm::normalize(m_ForwardDirection);
	}

	void Player::DeterminePlayerState()
	{
		const bool isWalking = Input::IsKeyPressed(GLFW_KEY_UP) || Input::IsKeyPressed(GLFW_KEY_DOWN);
		const bool isRunning = (Input::IsKeyPressed(GLFW_KEY_UP) || Input::IsKeyPressed(GLFW_KEY_DOWN)) && Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT);

		if (isRunning)
		{
			m_CurrentState = PlayerState::Run;
		}
		else if (isWalking)
		{
			m_CurrentState = PlayerState::Walk;
		}
		else
		{
			m_CurrentState = PlayerState::Idle;
		}
	}

	void Player::UpdateIdleState(float ts)
	{
		m_Color = { 0.0f, 1.0f, 0.0f };
	}

	void Player::UpdateWalkState(float ts)
	{
		m_Speed = 4.0f;
		m_Color = { 0.0f, 1.0f, 0.0f };

		HandleMovement(ts);
	}

	void Player::UpdateRunState(float ts)
	{
		m_Speed = 8.0f;

		m_RotationValue += 4.0f * ts;
		float CosValue = glm::abs(glm::cos(m_RotationValue));
		m_Color = { 0.0f, CosValue, 0.0f };

		HandleMovement(ts);
	}

	void Player::HandleMovement(float ts)
	{
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