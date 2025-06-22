#pragma once
#include "Events/Event.h"
#include "Events/MouseEvent.h"

#include "glm/glm.hpp"

#include "Core/Sandbox.h"
#include "Renderer/Shader.h"
#include <vector>
namespace JSG {

	class Sandbox2D : public Sandbox
	{
	public:
		Sandbox2D();
		virtual ~Sandbox2D() override;

		virtual void OnUpdate() override;
		virtual void OnRender() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
	private:

		glm::vec3 m_CircleColor = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_BColor = { 0.009f, 0.018f, 0.039f };
		// Shader Data
		uint32_t m_VertexArray;
		uint32_t m_VertexBuffer;
		uint32_t m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
		glm::vec3 m_ForwardDirection = { 0.0f, 0.0f, 0.0f };
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