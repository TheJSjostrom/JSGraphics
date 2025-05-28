#include "Core/Input.h"
#include "Core/Application.h"

 namespace JSG {

	bool Input::IsKeyPressed(uint32_t keyCode)
	{
		Application* app = Application::Get();
		const uint32_t state = glfwGetKey(app->GetWindow().GetNativeWindow(), keyCode);

		return state;
	}

	bool Input::IsMouseButtonPressed(uint32_t mouseCode)
	{
		Application* app = Application::Get();
		const uint32_t state = glfwGetMouseButton(app->GetWindow().GetNativeWindow(), mouseCode);
		
		return state;
	}

}