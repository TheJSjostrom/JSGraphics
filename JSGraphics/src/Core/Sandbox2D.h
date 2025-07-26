#pragma once
#include "Events/Event.h"
#include "Events/MouseEvent.h"

#include "Core/Sandbox.h"

#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/Camera.h"

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
	private:
		float ïVectorRotation = 0.0f;
		float ÿVectorRotation = -1.0f;

		// Camera
		float m_ZoomLevel = 10.0f;
		float m_AspectRatio = 0.0f;
		Camera m_Camera;

		// Shader Data
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
		glm::vec3 m_CameraUpDirection = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraVelocity = 10.0f;

		// Triangle
		glm::vec3 m_TrianglePosition = { 1.5f, -5.0f, 0.0f };
		float m_TriangleRotation = 0.0f;
		float m_TriangleSize = 2.0f;
		glm::vec3 m_TriangleColor = { 0.348f, 0.853f, 0.138f };

		// Quad
		glm::vec3 m_QuadPosition = { 10.0f, -5.0f, 0.0f };
		float m_QuadRotation = 0.0f;
		float m_QuadSize = 2.0f;
		glm::vec3 m_QuadColor = { 0.138f, 0.517f, 0.853f };

		// Circle
		float m_CircleSize = 2.0f;
		glm::vec3 m_CirclePosition = { 5.0f, -5.0f, 0.0f };
		glm::vec3 m_CircleColor = { 0.912f, 0.080f, 0.080f };

		// Player
		glm::vec3 m_PlayerPosition = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_PlayerColor = { 0.152f, 0.392f, 0.052f };
		glm::vec3 m_PlayerForwardDirection = { 1.0f, 0.0f, 0.0f };
		float m_PlayerVelocity = 10.0f;
		float m_PlayerSize = 1.0f;
		float m_PlayerSizeSpeed = 20.0f;
		float m_PlayerRotation = 0.0f;

		float m_QCColor = 0.5f;
		float m_GValue = 0.25f;
	};

}