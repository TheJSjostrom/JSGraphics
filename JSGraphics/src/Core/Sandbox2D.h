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
		const glm::vec3 CalculateLighting(const glm::vec3& position, const glm::vec3& color);
	private:
		float m_VectorBAngle = 0.0f;

		float m_VectorRotation = 45.0f;
		glm::vec3 m_Vector = { 0.0f, 0.0f, 0.0f };

		// Camera
		float m_ZoomLevel = 10.0f;
		float m_AspectRatio = 0.0f;
		Camera m_Camera;

		// Shader Data
		uint32_t m_LightCubeVertexArray;
		VertexBuffer m_LightCubeVertexBuffer;
		IndexBuffer m_LightCubeIndexBuffer;
		Shader m_LightCubeShader;

		uint32_t m_TriangleVertexArray;
		VertexBuffer m_TriangleVertexBuffer;
		IndexBuffer m_TriangleIndexBuffer;
		Shader m_TriangleShader;

		uint32_t m_QuadVertexArray;
		VertexBuffer m_QuadVertexBuffer;
		IndexBuffer m_QuadIndexBuffer;
		Shader m_QuadShader;

		uint32_t m_CircleVertexArray;
		VertexBuffer m_CircleVertexBuffer;
		IndexBuffer m_CircleIndexBuffer;
		Shader m_CircleShader;

		glm::vec3 m_BColor = { 0.009f, 0.018f, 0.039f };
	
		// Camera
		glm::vec3 m_CameraUpDirection = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_CameraPosition = { glm::cos(glm::radians(90.0f + -45.0f)) * 0.0f, glm::sin(glm::radians(90.0f + -45.0f)) * 0.0f, 0.0f};
		float m_CameraRotation = 0.0f;
		float m_CameraVelocity = 10.0f;

		// Quad
		glm::vec3 m_QuadPosition = { 10.0f, -5.0f, 0.0f };
		float m_QuadRotation = 0.0f;
		float m_QuadSize = 2.0f;
		glm::vec3 m_QuadColor = { 0.138f, 0.517f, 0.853f };

		// Circle
		float m_CircleSize = 2.0f;
		glm::vec3 m_CirclePosition = { 5.0f, -5.0f, 0.0f };
		glm::vec3 m_CircleColor = { 0.941f, 1.000f, 0.000f, };

		// Vector Circles
		std::vector<Circle> m_Circles;
		float m_VCircleSize = 2.0f;
		glm::vec3 m_VColor = { 1.0f, 1.0f, 1.0f };

		float m_Angle = 0.0f;

		// Floor
		glm::vec3 m_FloorPosition = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_FloorColor = { 0.941f, 1.000f, 0.000f };
		float m_FloorSize = 50.0f;
		float m_FloorRotation = 0.0f;

		// Enemy
		glm::vec3 m_EnemyPosition = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_EnemyForwardDirection = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_EnemyColor = { 1.0f, 0.0f, 0.0f };

		float m_EnemyVelocity = 4.0f;
		float m_EnemyFOVRange = 5.0f;
		float m_EnemyFOVAngle = 45.0f;
		float m_EnemyRotation = 0.0f;
		float m_EnemySize = 1.0f;
		glm::vec3 m_Result2 = { 0.0f, 0.0f, 0.0f };
		bool m_AttackState = false;
		float m_Angle2 = 0.0f;
		float m_NormalizedDifferencePEAngle = 0.0f;

		// Player
		glm::vec3 m_PlayerPosition = { 1.0f, 2.0f, 0.0f };
		glm::vec3 m_PlayerColor = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_PlayerForwardDirection = { 1.0f, 0.0f, 0.0f };
		float m_PlayerRotationVelocity = 180.0f;
		float m_PlayerVelocity = 8.0f;
		float m_PlayerSizeVelocity = 1.0f;
		float m_PlayerSize = 1.0f;
		float m_PlayerRotation = 0.0f;
		glm::vec3 m_Result = { 0.0f, 0.0f, 0.0f };
		float m_QCColor = 0.5f;

		// Light Cube
		glm::vec3 m_LightCubeColor = { 1.0f, 1.0f, 1.0f };
		glm::vec3 m_LigthCubeDirection = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_LightCubePosition = { -2.0f, 1.0f, 0.5f };
		float m_LightCubeAngle = 90.0f;
	};

}