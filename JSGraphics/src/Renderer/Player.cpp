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
		case PlayerState::Sprint:
			UpdateRunState(ts); 
			break;
		}

		HandleMovement(ts);
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
		const glm::vec3 forward = { glm::cos(glm::radians(m_Rotation)), glm::sin(glm::radians(m_Rotation)), 0.0f };
		m_ForwardDirection = glm::normalize(forward);
	}

	void Player::DeterminePlayerState()
	{
		if (IsSprinting())
		{
			m_CurrentState = PlayerState::Sprint;
		}
		else if (IsWalking())
		{
			m_CurrentState = PlayerState::Walk;
		}
		else
		{
			m_CurrentState = PlayerState::Idle;
		}
	}

	bool Player::IsWalking() const
	{
		return Input::IsKeyPressed(GLFW_KEY_UP) || Input::IsKeyPressed(GLFW_KEY_DOWN);
	}

	bool Player::IsSprinting() const
	{
		return IsWalking() && Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT);
	}

	void Player::UpdateIdleState(float ts)
	{
		m_Color = { 0.0f, 0.5f, 0.0f };
	}

	void Player::UpdateWalkState(float ts)
	{
		m_Speed = 4.0f;
		m_Color = { 0.0f, 0.5f, 0.0f };
	}

	void Player::UpdateRunState(float ts)
	{
		m_Speed = 8.0f;
		m_PulseSpeed = 4.0f;

		UpdateColorPulse(ts, 0);
	}

	bool Player::IsOutOfBound() const
	{
		return m_Position.x > 25.0f || m_Position.x < -25.0f ||
			   m_Position.y > 26.0f || m_Position.y < -25.0f;
	}

	void Player::UpdateColorPulse(float ts, uint32_t color)
	{
		m_PulseTimer += m_PulseSpeed * ts;
		const float colorIntensity = glm::abs(glm::cos(m_PulseTimer));
	
		switch (color)
		{
		case 0:
			m_Color = { 0.0f, colorIntensity, 0.0f };
			break;
		case 1:
			m_Color = { colorIntensity, 0.0f, 0.0f };
			break;
		}
	}

	glm::vec3 Player::CalculateVelocity(float ts) const
	{
		return m_ForwardDirection * m_Speed * ts;
	}

	void Player::HandleMovement(float ts)
	{
		const glm::vec3 velocity = CalculateVelocity(ts);

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