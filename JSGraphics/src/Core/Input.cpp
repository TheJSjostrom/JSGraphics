#include "Core/Input.h"
#include "Core/Application.h"

 namespace JSG {

	bool Input::IsKeyPressed(uint32_t keyCode)
	{
		Application& app = *Application::Get();
		const uint32_t state = glfwGetKey(app.GetWindow().GetNativeWindow(), keyCode);

		return state;
	}

	bool Input::IsMouseButtonPressed(uint32_t mouseCode)
	{
		Application& app = *Application::Get();
		const uint32_t state = glfwGetMouseButton(app.GetWindow().GetNativeWindow(), mouseCode);
		
		return state;
	}

	glm::vec2 Input::GetMousePosition()
	{
		Application& app = *Application::Get();
		double xPos, yPos;
		glfwGetCursorPos(app.GetWindow().GetNativeWindow(), &xPos, &yPos);
		
		return { static_cast<float>(xPos), static_cast<float>(yPos) };
	}

	float Input::GetMousePositionX()
	{
		return GetMousePosition().x;
	}

	float Input::GetMousePositionY()
	{
		return GetMousePosition().y;
	}

}