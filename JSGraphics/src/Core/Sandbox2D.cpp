#include "Core/Sandbox2D.h"
#include "Core/Window.h"
#include "Core/Application.h"
#include "Core/Core.h"
#include "Core/Input.h"

#include "Events/MouseEvent.h"

#include "imgui.h"

#include "glad/glad.h"

#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include <algorithm>

namespace JSG {

	Sandbox2D::Sandbox2D()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glCreateVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[4 * 3] = {
     	// Vertex Position
		   -0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f, 
			0.5f,  0.5f, 0.0f,
		   -0.5f,  0.5f, 0.0f
		};

		m_VertexBuffer.Init(sizeof(vertices), vertices);
		m_VertexBuffer.Bind();

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		
		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		m_IndexBuffer.Init(sizeof(indices), indices);
		m_IndexBuffer.Bind();

		const std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_Proj;
			uniform mat4 u_View;
			uniform mat4 u_Model;

			void main()
			{
				gl_Position = u_Proj * u_View * u_Model * vec4(a_Position, 1.0);
			}
		)";

		const std::string fragSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform vec3 u_Color;

			void main()
			{ 
				color = vec4(u_Color, 1.0);
			}
		)";

		m_Shader.Init(vertexSrc, fragSrc);
		m_Shader.Bind();

		// Renderer Circle
		glCreateVertexArrays(1, &m_CircleVertexArray);
		glBindVertexArray(m_CircleVertexArray);

		float CircleVertices[4 * 6] = {
			// Vertex Position     Local Position
			   -0.5f, -0.5f, 0.0f, -1.0f, -1.0f, 0.0f,
				0.5f, -0.5f, 0.0f,  1.0f, -1.0f, 0.0f,
				0.5f,  0.5f, 0.0f,  1.0f,  1.0f, 0.0f,
			   -0.5f,  0.5f, 0.0f, -1.0f,  1.0f, 0.0f
		};

		m_CircleVertexBuffer.Init(sizeof(CircleVertices), CircleVertices);
		m_CircleVertexBuffer.Bind();

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));

		uint32_t CircleIndices[6] = { 0, 1, 2, 2, 3, 0 };
		m_CircleIndexBuffer.Init(sizeof(CircleIndices), CircleIndices);
		m_CircleIndexBuffer.Bind();

		const std::string CircleVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_LocalPosition;
			
			out vec3 FragPos;

			uniform mat4 u_Proj;
			uniform mat4 u_View;
			uniform mat4 u_Model;

			void main()
			{
				FragPos = a_LocalPosition;
				gl_Position = u_Proj * u_View * u_Model * vec4(a_Position, 1.0);
			}
		)";

		const std::string CircleFragSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 FragPos;

			uniform vec3 u_Color;

			void main()
			{ 
				float FragPosLength = sqrt(FragPos.x * FragPos.x + FragPos.y * FragPos.y);
				color = vec4(u_Color, 0.0);

				if (FragPosLength < 1.0)
					color = vec4(u_Color, 1.0f);
			}
		)";

		m_CircleShader.Init(CircleVertexSrc, CircleFragSrc);
		m_CircleShader.Bind();
	}

	Sandbox2D::~Sandbox2D()
	{
	}

	void Sandbox2D::OnUpdate(float ts)
	{
		// Aspect Ratio
		Application& app = *Application::Get();
		m_AspectRatio = app.GetWindow().GetWidth() / static_cast<float>(app.GetWindow().GetHeight());

		// Player
		glm::vec3 PlayerUpDirection = { 0.0f, 0.0f, 1.0f };
		glm::vec3 PlayerLeftDirection = glm::normalize(glm::cross(PlayerUpDirection, m_PlayerForwardDirection));

		m_PlayerForwardDirection.x = glm::cos(glm::radians(m_PlayerRotation));
		m_PlayerForwardDirection.y = glm::sin(glm::radians(m_PlayerRotation));
		m_PlayerForwardDirection.z = 0.0f;
		m_PlayerForwardDirection = glm::normalize(m_PlayerForwardDirection);

		if (Input::IsKeyPressed(GLFW_KEY_UP))
		{
			m_PlayerPosition.x += m_PlayerForwardDirection.x * m_PlayerVelocity * ts;
			m_PlayerPosition.y += m_PlayerForwardDirection.y * m_PlayerVelocity * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_DOWN))
		{
			m_PlayerPosition.x -= m_PlayerForwardDirection.x * m_PlayerVelocity * ts;
			m_PlayerPosition.y -= m_PlayerForwardDirection.y * m_PlayerVelocity * ts;
		}

		if (Input::IsKeyPressed(GLFW_KEY_LEFT))
		{
			m_PlayerRotation += 180.0f * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_RIGHT))
		{
			m_PlayerRotation -= 180.0f * ts;
		}

		if (Input::IsKeyPressed(GLFW_KEY_C))
		{
			m_PlayerSize += 5.0f * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_V))
		{
			m_PlayerSize -= 5.0f * ts;
		}

		if (m_PlayerSize <= 0.5f)
			m_PlayerSize = 0.5f;

		// Camera
		glm::vec3 CameraBackwardDirection = { 0.0f, 0.0f, 1.0f };
		glm::vec3 CameraleftDirection = glm::normalize(glm::cross(m_CameraForwardDirection, CameraBackwardDirection));

		m_CameraForwardDirection.x = glm::cos(glm::radians(m_CameraRotation + 90.0f));
		m_CameraForwardDirection.y = glm::sin(glm::radians(m_CameraRotation + 90.0f));
		m_CameraForwardDirection.z = 0.0f;
		m_CameraForwardDirection = glm::normalize(m_CameraForwardDirection);

		if (Input::IsKeyPressed(GLFW_KEY_W)) 
		{
			m_CameraPosition.x += m_CameraForwardDirection.x * m_CameraVelocity * ts;
			m_CameraPosition.y += m_CameraForwardDirection.y * m_CameraVelocity * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_S))
		{
			m_CameraPosition.x -= m_CameraForwardDirection.x * m_CameraVelocity * ts;
			m_CameraPosition.y -= m_CameraForwardDirection.y * m_CameraVelocity * ts;
		}

		if (Input::IsKeyPressed(GLFW_KEY_D))
		{
			m_CameraPosition.x += CameraleftDirection.x * m_CameraVelocity * ts;
			m_CameraPosition.y += CameraleftDirection.y * m_CameraVelocity * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_A))
		{
			m_CameraPosition.x -= CameraleftDirection.x * m_CameraVelocity * ts;
			m_CameraPosition.y -= CameraleftDirection.y * m_CameraVelocity * ts;
		}

		if (Input::IsKeyPressed(GLFW_KEY_Q))
		{
			m_CameraRotation += 180.0f * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_E))
		{
			m_CameraRotation -= 180.0f * ts;
		}

		if (m_Switch)
		{
			m_Color += 1.0f;

			if (m_Color > 1.0f)
				m_Switch = false;

		}
		else if (!m_Switch)
		{
			m_Color -= 1.0f;

			if (m_Color < 0.1f)
				m_Switch = true;

		}

		// Dot Product math
		/*
		if (Input::IsKeyPressed(GLFW_KEY_A) && Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		{
			m_VAngle += 180 * (1 / 60.0f);
		}
		else if (Input::IsKeyPressed(GLFW_KEY_D) && Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		{
			m_VAngle -= 180 * (1 / 60.0f);
		}
		*/
		m_V = { glm::cos(glm::radians(m_VAngle)), glm::sin(glm::radians(m_VAngle)) };
		m_V2 = { glm::cos(glm::radians(m_V2Angle)) * 5.09901953f, glm::sin(glm::radians(m_V2Angle)) * 5.09901953f };

		float LengthVector = glm::sqrt(m_V.x * m_V.x + m_V.y * m_V.y);
	    float LengthVector2 = glm::sqrt(m_V2.x * m_V2.x + m_V2.y * m_V2.y);
		float x = m_V.x * m_V2.x;
		float y = m_V.y * m_V2.y;
		m_DotProduct = m_V.x * m_V2.x + m_V.y * m_V2.y;
		m_DotProduct2 = glm::cos(glm::radians(m_VAngle - m_V2Angle)) * 5.09901953f * glm::sqrt(m_V.x * m_V.x + m_V.y * m_V.y);

		// Rotation math
		glm::vec2 iVector = { glm::cos(glm::radians(m_Angle)), glm::sin(glm::radians(m_Angle)) };
		m_IVector = iVector;
		glm::vec2 jVector = { -glm::sin(glm::radians(m_Angle)), glm::cos(glm::radians(m_Angle)) };
		glm::vec2 pos = { 0.5f, 0.5f };

		glm::vec2 transformedPos = pos.x * iVector + pos.y * jVector;
		const float transformedPosLength = glm::sqrt(transformedPos.x * transformedPos.x + transformedPos.y * transformedPos.y);
		glm::vec2 transformedPosUV = { transformedPos.x / transformedPosLength, transformedPos.y / transformedPosLength };
		m_PosUV = transformedPosUV;

		if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1) && Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		{
			m_Angle += 1.0f * ts;
		}
		else if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2) && Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		{
			m_Angle -= 1.0f * ts;
		}
		// x = .999848    y = .017452
    	//	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_Angle), glm::vec3(0.0f, 0.0f, 1.0f));
		// 
		// [cos(45.0f) -sin(45.0f)]
		// [sin(45.0f)  cos(45.0f)]
		//std::cout << Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1) << std::endl;

	    //  glm::vec4 position = { 0.5f, 0.5f, 0.0f, 1.0f};
		
    	//	glm::vec4 transformedPosition = rotationMatrix * position;
		// position.x * [cos(45.0f)] + position.y * [-sin(45.0f)]
		//              [sin(45.0f)]                [ cos(45.0f)]

		// [1 0 0 2]  {0.5}        {1}       {0}     {0}   {2}       {0.5}   {2} 
		// [0 1 0 3]  {0.5}	   0.5*{0} + 0.5*{1} + 0*{0} + {3}       {0.5} + {3} 
		// [0 0 1 0]  {0}          {0}       {0}     {1}   {0}       {0}     {0}
		//
	}

	void Sandbox2D::OnRender()
	{
		// Clear the the whole scene.
		glClearColor(m_BColor.x, m_BColor.y, m_BColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 projectionMatrix = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -1.0f, 1.0f);
		glm::mat4 viewMatrix = glm::translate(glm::mat4(1), m_CameraPosition) 
							 * glm::rotate(glm::mat4(1.0f), glm::radians(m_CameraRotation), glm::vec3(0, 0, 1));
		viewMatrix = glm::inverse(viewMatrix);
		
		// Entities |
		//			v
		// Render quad player
		{
			m_Shader.Bind();
			m_Shader.SetMat4("u_Proj", projectionMatrix);
			m_Shader.SetMat4("u_View", viewMatrix);

			glm::mat4 modelMatrix = glm::translate(glm::mat4(1), m_PlayerPosition) 
								  * glm::rotate(glm::mat4(1.0f), glm::radians(m_PlayerRotation), glm::vec3(0, 0, 1)) 
								  * glm::scale(glm::mat4(1), glm::vec3(m_PlayerSize, m_PlayerSize, m_PlayerSize));
			m_Shader.SetMat4("u_Model", modelMatrix);
			m_Shader.SetFloat3("u_Color", m_PlayerColor);

			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}

		// Render a quad.
		{
			m_Shader.Bind();
			m_Shader.SetMat4("u_Proj", projectionMatrix);
			m_Shader.SetMat4("u_View", viewMatrix);
			
			glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 0.0f))
								  * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f))
				                  * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			m_Shader.SetMat4("u_Model", modelMatrix);
			m_CircleShader.SetFloat3("u_Color", { 0.25f, 0.1f, 0.20f });

			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}


		// Render a circle.
		{
			m_CircleShader.Bind();
			m_CircleShader.SetMat4("u_Proj", projectionMatrix);
			m_CircleShader.SetMat4("u_View", viewMatrix);

			glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(m_CircleXPos, m_CircleYPos, 0.0f))
				* glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f))
				* glm::scale(glm::mat4(1.0f), glm::vec3(m_CircleSize, m_CircleSize, m_CircleSize));
			m_CircleShader.SetMat4("u_Model", modelMatrix);
			m_CircleShader.SetFloat3("u_Color", m_CircleColor);

			glBindVertexArray(m_CircleVertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}

		// Render a quad made of circles
		{
			for (uint32_t i = 0; i < 20; i++)
			{
				for (uint32_t y = 0; y < 20; y++)
				{
					glm::mat4 modelMatrix = glm::translate(glm::mat4(1), {i, y, 0.0f}) 
										  * glm::rotate(glm::mat4(1.0f), glm::radians(m_PlayerRotation), glm::vec3(0.0f, 0.0f, 1.0f)) 
										  * glm::scale(glm::mat4(1), glm::vec3(0.1f, 0.1f, 0.1f));
					m_CircleShader.SetMat4("u_Model", modelMatrix);
					m_CircleShader.SetFloat3("u_Color", { 1.0f, 0.0f, 1.0f * i/static_cast<float>(20)});

					glBindVertexArray(m_CircleVertexArray);
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
				}
			}
		}

		// Render a dot in origo
		{
			glm::mat4 projMatrix = glm::ortho(-m_AspectRatio, m_AspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
			glm::mat4 viewMatrix = glm::translate(glm::mat4(1), {0.0f, 0.0f, 0.0f});
			viewMatrix = glm::inverse(viewMatrix);

			m_CircleShader.Bind();
			m_CircleShader.SetMat4("u_Proj", projMatrix);
			m_CircleShader.SetMat4("u_View", viewMatrix);

			glm::mat4 modelMatrix = glm::translate(glm::mat4(1), { 0.0f, 0.0f, 0.0f }) 
								   * glm::scale(glm::mat4(1), glm::vec3(0.009f, 0.009f, 0.009f));
			m_CircleShader.SetMat4("u_Model", modelMatrix);
			m_CircleShader.SetFloat3("u_Color", {1.0f, 1.0f, 1.0f});

			glBindVertexArray(m_CircleVertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}

	}

	void Sandbox2D::OnImGuiRender()
	{
		Application* app = Application::Get();

		//ImGui::ShowDemoWindow();
		ImGui::Begin("Camera Information");
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "CAMERA CONTROLS");
		ImGui::Text("Move the camera around: WASD");
		ImGui::Text("Zoom in and out: Mouse Scroll Wheel");
		ImGui::Text("Rotate the camera: QE");
		ImGui::Text(" ");
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "CAMERA INFORMATION");
		ImGui::Text("Camera Rotation Angle: %f", m_CameraRotation);
		ImGui::Text("Camera Position: { %f, %f, %f }", m_CameraPosition.x, m_CameraPosition.y, m_CameraPosition.z);
		ImGui::End();

		ImGui::Begin("Settings");
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "COLOR");
		ImGui::ColorEdit4("Background Color", reinterpret_cast<float*>(&m_BColor));
		ImGui::ColorEdit4("Player Color", reinterpret_cast<float*>(&m_PlayerColor));
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Aspect Ratio");
		ImGui::Text("Window Height: %f", static_cast<float>(app->GetWindow().GetHeight()));
		ImGui::Text("Wondow Width: %f", static_cast<float>(app->GetWindow().GetWidth()));
		ImGui::Text("Window Aspect Ratio: %f", m_AspectRatio);

		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Circle Settings");
		ImGui::SliderFloat("Size: ", &m_CircleSize, 1.0f, 100.0f);
		ImGui::SliderFloat("X position", &m_CircleXPos, -100.0f, 100.0f);
		ImGui::SliderFloat("Y position", &m_CircleYPos, -100.0f, 100.0f);
		ImGui::ColorEdit4("Circle Color: ", reinterpret_cast<float*>(&m_CircleColor));
		ImGui::End();

		ImGui::Begin("Math");
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "ROTATION");
		ImGui::SliderFloat("Change Angle", &m_Angle, 0.0f, 360.0f);
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "IVector");
		ImGui::Text("IVector X: %f", m_IVector.x);
		ImGui::Text("IVector Y: %f", m_IVector.y);
		ImGui::Text("IVector Angle: %f", m_Angle);

		ImGui::TextColored(ImVec4(1, 0, 0, 1), "transformedPos");
		ImGui::Text("TransformedPos VectorX: %f", m_PosUV.x);
		ImGui::Text("TransformedPos VectorY: %f", m_PosUV.y);
		ImGui::Text("IVector Angle: %f", m_Angle);

		ImGui::Text(" ");
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "DOT PRODUCT");
		ImGui::SliderFloat("Change VAngle", &m_VAngle, 0.0f, 360.0f);
		ImGui::SliderFloat("Change V2Angle", &m_V2Angle, 0.0f, 360.0f);
		ImGui::Text("Dot Product: %f", m_DotProduct);
		ImGui::Text("Dot Product2: %f", m_DotProduct2);
		ImGui::Text("Vector: %f, %f", m_V.x, m_V.y);
		ImGui::Text("Vector2: %f, %f", m_V2.x, m_V2.y);
		ImGui::End();
	}

	void Sandbox2D::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(Sandbox2D::OnMouseScrolled));
	}

	bool Sandbox2D::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel += e.GetYOffset() * 0.5f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.50f);
		return false;
	}
}
