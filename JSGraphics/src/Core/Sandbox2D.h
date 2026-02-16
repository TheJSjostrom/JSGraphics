#pragma once
#include "Events/Event.h"
#include "Events/MouseEvent.h"

#include "Core/Sandbox.h"

#include "Renderer/Texture2D.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/Camera.h"
#include "Renderer/Circle.h"
#include "Renderer/Ball.h"
#include "Renderer/Enemy.h"
#include "Renderer/Player.h"

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
		virtual void OnEvent(Event& e) override;

		virtual void OnImGuiRender() override;
	private:
		bool OnMouseScrolled(const MouseScrolledEvent& e);
		bool OnMouseButtonPressed(const MouseButtonPressedEvent& e);

		void CalculateDotProductAngle() const;
	private:
		glm::vec3 m_BackgroundColor = { 0.009f, 0.018f, 0.039f };

		Texture2D m_Texture = std::string("assets/texture/wooden.jpg");

		// Shader Data
		uint32_t m_TextureQuadVertexArray;
		VertexBuffer m_TextureQuadVertexBuffer;
		IndexBuffer m_TextureQuadIndexBuffer;
		Shader m_TextureQuadShader;
		
		uint32_t m_TriangleVertexArray;
		VertexBuffer m_TriangleVertexBuffer;
		IndexBuffer m_TriangleIndexBuffer;
		Shader m_TriangleShader;

		uint32_t m_BallVertexArray;
		VertexBuffer m_BallVertexBuffer;
		IndexBuffer m_BallIndexBuffer;
		Shader m_BallShader;

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

		// Camera
		Camera m_Camera;
		float m_ZoomLevel = 1.0f;
		float m_AspectRatio = 0.0f;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraSpeed = 10.0f;

		// Vector Circles
		std::vector<Circle> m_Circles;
		glm::vec3 m_VColor = { 1.0f, 1.0f, 1.0f };

		float m_VCircleSize = 2.0f;

		// Ball
		std::vector<Ball> m_Balls;

		// Player
		Player m_Player;

		// Enemy
		Enemy m_Enemy;

		// Light Cube
		glm::vec3 m_LightCubeColor = { 1.0f, 1.0f, 1.0f };
		glm::vec3 m_LigthCubeDirection = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_LightCubePosition = { 1.0f, 5.0f, 0.5f };
		float m_LightCubeAngle = 90.0f;

		// Floor
		glm::vec3 m_FloorColor = { 0.941f, 1.000f, 0.000f };
 
		float m_FloorSize = 50.0f;
		float m_FloorRotation = 0.0f;
	};

}