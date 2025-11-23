#include "Player.h"
#include "GLFW/glfw3.h"
#include "Core/Input.h"

namespace JSG {

	void Player::OnUpdate(float ts)
	{
		HandleRotation(ts);
		UpdateForwardDirection();
		DeterminePlayerState();
		HandleMovement(ts);

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
		m_Color = { 0.0f, 0.5f, 0.0f };
	}

	void Player::UpdateRunState(float ts)
	{
		m_Speed = 8.0f;
		m_PulseTimer += 4.0f * ts;
		const float colorIntensity = glm::abs(glm::cos(m_PulseTimer));
		m_Color = { 0.0f, colorIntensity, 0.0f };
	}

	void Player::HandleMovement(float ts)
	{
		const glm::vec3 velocity = m_ForwardDirection * m_Speed * ts;

		if (Input::IsKeyPressed(GLFW_KEY_UP))
		{
			m_Position += velocity;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_DOWN))
		{
			m_Position -= velocity;
		}
	}
}