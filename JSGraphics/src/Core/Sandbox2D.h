#pragma once
#include "Events/Event.h"
#include "Events/MouseEvent.h"

#include "Core/Sandbox.h"

#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"

#include "glm/glm.hpp"

#include <vector>

namespace JSG {

	class Sandbox2D : public Sandbox
	{
	public:
		Sandbox2D();
		virtual ~Sandbox2D() override;

		virtual void OnUpdate(float ts) override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
	private:
		// Shader Data
		uint32_t m_VertexArray;
		VertexBuffer m_VertexBuffer;
		IndexBuffer m_IndexBuffer;
		Shader m_Shader;

		uint32_t m_CircleVertexArray;
		VertexBuffer m_CircleVertexBuffer;
		IndexBuffer m_CircleIndexBuffer;
		Shader m_CircleShader;

		float m_Color = 0.0f;
		bool m_Switch = true;
 
		glm::vec3 m_BColor = { 0.009f, 0.018f, 0.039f };

		// Camera
		glm::vec3 m_CameraForwardDirection = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraVelocity = 10.0f;

		// Circle
		float m_CircleSize = 1.0f;
		glm::vec3 m_CircleColor = { 0.5f, 0.0f, 0.25f };
		float m_CircleXPos = 5.0f;
		float m_CircleYPos = -5.0f;

		// Player
		glm::vec3 m_PlayerPosition = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_PlayerColor = { 0.25f, 0.0f, 0.25f };
		glm::vec3 m_PlayerForwardDirection = { 1.0f, 0.0f, 0.0f };
		float m_PlayerVelocity = 10.0f;
		float m_PlayerSize = 1.0f;
		float m_PlayerRotation = 0.0f;

		std::vector<int> m_Number;
		std::vector<int> m_Number2;

		float m_ZoomLevel = 1.0f;
		float m_AspectRatio = 0.0f;
		float m_GValue = 0.25f;

		// Math Data
		glm::vec2 m_PosUV = { 0.0f, 0.0f };
		glm::vec2 m_IVector = { 0.0f, 0.0f };

		glm::vec2 m_V = { 0.0f, 0.0f };
		glm::vec2 m_V2 = { 0.0f, 0.0f };
		float m_DotProduct2 = 0.0f;
		float m_DotProduct = 0.0f;
		float m_VAngle = 45.0f;
		float m_V2Angle = 10.0f;
		float m_XPos = 0.0f;
		float m_YPos = 0.0f;
		float m_Angle = 0.0f;
		float m_Length = 0.0f;
		float m_LengthTrans = 0.0f;

		glm::vec2 m_Pos = { 0.0f, 0.0f };
	};

}