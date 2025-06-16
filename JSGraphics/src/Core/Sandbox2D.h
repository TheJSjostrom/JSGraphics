#pragma once
#include "Events/Event.h"
#include "Events/MouseEvent.h"

#include "glm/glm.hpp"

#include "Core/Sandbox.h"
#include "Renderer/Shader.h"

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
		glm::vec3 m_Color = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_BColor = { 1.0f, 0.0f, 0.0f };
		uint32_t m_VertexArray;
		uint32_t m_VertexBuffer;
		Shader m_Shader;

		float m_AspectRatio = 0.0f;
		float m_GValue = 0.25f;

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