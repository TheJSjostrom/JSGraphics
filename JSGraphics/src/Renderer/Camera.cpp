#include "Camera.h"

namespace JSG {

	Camera::Camera(float left, float right, float bottom, float top) :
		m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
	{
		UpdateViewMatrix();
	}

	void Camera::UpdateViewMatrix()
	{
		const glm::mat4 CameraMatrix = glm::translate(glm::mat4(1.0f), m_Position) 
									 * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		m_ViewMatrix = glm::inverse(CameraMatrix);
	}
}