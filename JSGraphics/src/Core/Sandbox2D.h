#pragma once
#include "Events/Event.h"
#include "Events/MouseEvent.h"

#include "Core/Sandbox.h"

#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/Camera.h"
#include "Renderer/Circle.h"

#include "glm/glm.hpp"

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
		bool OnMouseScrolled(const MouseScrolledEvent& e);
		bool OnMouseButtonPressed(const MouseButtonPressedEvent& e);
	private:
		float m_VectorRotation = 45.0f;
		glm::vec3 m_Vector = { 0.0f, 0.0f, 0.0f };

		// Shader Data
		uint32_t m_LightCubeVertexArray;
		VertexBuffer m_LightCubeVertexBuffer;
		IndexBuffer m_LightCubeIndexBuffer;
		Shader m_LightCubeShader;

		uint32_t m_QuadVertexArray;
		VertexBuffer m_QuadVertexBuffer;
		IndexBuffer m_QuadIndexBuffer;
		Shader m_QuadShader;

		uint32_t m_CircleVertexArray;
		VertexBuffer m_CircleVertexBuffer;
		IndexBuffer m_CircleIndexBuffer;
		Shader m_CircleShader;

		glm::vec3 m_BackgroundColor = { 0.009f, 0.018f, 0.039f };
	
		// Camera
		Camera m_Camera;
		float m_ZoomLevel = 10.0f;
		float m_AspectRatio = 0.0f;

		glm::vec3 m_CameraUpDirection = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraVelocity = 10.0f;

		// Vector Circles
		std::vector<Circle> m_Circles;
		float m_VCircleSize = 2.0f;
		glm::vec3 m_VColor = { 1.0f, 1.0f, 1.0f };

		// Player
		glm::vec3 m_PlayerPosition = { 0.0f, 5.0f, 0.0f };
		glm::vec3 m_PlayerColor = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_PlayerForwardDirection = { 0.0f, 1.0f, 0.0f };

		float m_PlayerRotationVelocity = 180.0f;
		float m_PlayerVelocity = 8.0f;
		float m_PlayerSizeVelocity = 1.0f;
		float m_PlayerSize = 1.0f;
		float m_PlayerRotation = 90.0f;
		float m_PlayerHitBox = 1.25f;

		// Enemy
		glm::vec3 m_EnemyPosition = { -5.0f, 0.0f, 0.0f };
		glm::vec3 m_EnemyForwardDirection = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_EnemyColor = { 1.0f, 0.0f, 0.0f };

		float m_EnemyVelocity = 4.0f;
		float m_EnemyFOVRange = 5.0f;
		float m_EnemyFOVAngle = 45.0f;
		float m_EnemyRotation = -90.0f;
		float m_EnemySize = 1.0f;
		bool m_EnemyAttackState = false;
		float m_RotationValue = 0.0f;

		// Light Cube
		glm::vec3 m_LightCubeColor = { 1.0f, 1.0f, 1.0f };
		glm::vec3 m_LigthCubeDirection = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_LightCubePosition = { 1.0f, 5.0f, 0.5f };
		float m_LightCubeAngle = 90.0f;

		// Floor
		glm::vec3 m_FloorPosition = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_FloorColor = { 0.941f, 1.000f, 0.000f };

		float m_FloorSize = 50.0f;
		float m_FloorRotation = 0.0f;
	};

}