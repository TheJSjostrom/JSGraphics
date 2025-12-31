#include "Player.h"
#include "GLFW/glfw3.h"
#include "Core/Input.h"

namespace JSG {

	void Player::OnUpdate(float ts)
	{
		HandleRotation(ts);
		UpdateForwardDirection();
		DeterminePlayerState();
		//UpdateJumpPhysics(ts);
		// r = v - 2 * dot(v, n) * n
		
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

		HandleMovement(ts);
	}

	void Player::Jump(float ts)
	{
		if (m_IsOnGround) // Only allow jumping if on the ground
		{
			m_VerticalVelocity = m_JumpForce; // Apply immediate upward velocity
			m_IsOnGround = false;             // We are now airborne
		}
	}

	void Player::UpdateJumpPhysics(float ts)
	{
		// Apply gravity (pulls velocity down over time)
		m_VerticalVelocity -= m_Gravity * ts;

		// Apply the vertical velocity to the position
		// Assuming m_Position is a glm::vec3/vec2, you need to apply to the Y axis
		m_Position.y += m_VerticalVelocity * ts;

		// A very basic "ground collision" check (adjust for your specific game's floor level)
		if (m_Position.y <= 0.0f) {
			m_Position.y = 0.0f;        // Restrict position to ground level
			m_IsOnGround = true;        // Reset the ground flag
			m_VerticalVelocity = 0.0f;  // Stop vertical movement
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
		const glm::vec3 forward = { glm::cos(glm::radians(m_Rotation)), glm::sin(glm::radians(m_Rotation)), 0.0f };
		m_ForwardDirection = glm::normalize(forward);
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
		const bool isOutOfBound = m_Position.x >  25.0f || 
								  m_Position.x < -25.0f ||
								  m_Position.y >  26.0f || 
							      m_Position.y < -25.0f;

		return isOutOfBound;
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