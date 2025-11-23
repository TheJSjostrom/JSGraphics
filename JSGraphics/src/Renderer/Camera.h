#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace JSG {

	class Camera
	{
	public:
		Camera(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; UpdateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; UpdateViewMatrix(); }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		
		void SetProjectionMatrix(float left, float right, float bottom, float top) { m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f); }
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
	private:
		void UpdateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix = 1.0f;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};

}