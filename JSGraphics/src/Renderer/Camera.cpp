#include "Camera.h"

namespace JSG {

	Camera::Camera(float left, float right, float bottom, float top) : 
		m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
	{
	}

	void Camera::CalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1), m_Position)
						    * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		m_ViewMatrix = glm::inverse(transform);
	}
}