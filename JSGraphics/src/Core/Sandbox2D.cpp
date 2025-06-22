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

		glCreateBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		float SquareVertices[4 * 6] = {
     	// Vertex Position     Local Position
		   -0.5f, -0.5f, 0.0f, -1.0f, -1.0f, 0.0f,
			0.5f, -0.5f, 0.0f,  1.0f, -1.0f, 0.0f,
			0.5f,  0.5f, 0.0f,  1.0f,  1.0f, 0.0f,
		   -0.5f,  0.5f, 0.0f, -1.0f,  1.0f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(SquareVertices), reinterpret_cast<void*>(SquareVertices), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
		
		glCreateBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		
		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), (void*)indices, GL_STATIC_DRAW);

		const std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_LocalPosition;
			
			out vec3 FragPos;

			uniform vec3 u_Position;
			uniform mat4 u_Proj;
			uniform mat4 u_View;
			
			void main()
			{
				FragPos = a_LocalPosition;
				gl_Position = u_Proj * u_View * vec4(a_Position.x + u_Position.x, a_Position.y + u_Position.y, 0.0, 1.0);
			}
		)";

		const std::string fragSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 FragPos;

			uniform vec3 u_Color;

			void main()
			{ 
				float FragPosLength = sqrt(FragPos.x * FragPos.x + FragPos.y * FragPos.y);
				
				if (FragPosLength < 1.0)
					color = vec4(u_Color, 1.0);
			}
		)";

		m_Shader = std::make_unique<Shader>(vertexSrc, fragSrc);
		m_Shader->Bind();

		for (uint32_t i = 0; i < 100; i++)
		{
			m_Number.push_back(rand() % 100);
		}
		for (uint32_t i = 0; i < 100; i++)
		{
			m_Number2.push_back(rand() % 100);
		}
	}

	Sandbox2D::~Sandbox2D()
	{
	}

	void Sandbox2D::OnUpdate()
	{
		m_Shader->SetFloat3("u_Color", m_CircleColor);
 
		// Aspect Ratio
		Application& app = *Application::Get();
		m_AspectRatio = app.GetWindow().GetWidth() / static_cast<float>(app.GetWindow().GetHeight());

		// Shader
		m_Shader->SetMat4("u_Proj", glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -1.0f, 1.0f));

		glm::vec3 backwardDirection = { 0.0f, 0.0f, 1.0f };
		glm::vec3 leftDirection = glm::normalize(glm::cross(backwardDirection, m_ForwardDirection));

		m_ForwardDirection.x = glm::cos(glm::radians(m_Rotation + 90.0f));
		m_ForwardDirection.y = glm::sin(glm::radians(m_Rotation + 90.0f));
		m_ForwardDirection.z = 0.0f;
		m_ForwardDirection = glm::normalize(m_ForwardDirection);

		if (Input::IsKeyPressed(GLFW_KEY_W)) 
		{
			m_Position.x += m_ForwardDirection.x * 10.0f * 1 / static_cast<float>(60);
			m_Position.y += m_ForwardDirection.y * 10.0f *  1 / static_cast<float>(60);
		}
		else if (Input::IsKeyPressed(GLFW_KEY_S))
		{
			m_Position.x -= m_ForwardDirection.x * 10.0f * 1 / static_cast<float>(60);
			m_Position.y -= m_ForwardDirection.y * 10.0f *  1 / static_cast<float>(60);
		}

		if (Input::IsKeyPressed(GLFW_KEY_A))
		{
			m_Position.x += leftDirection.x * 10.0f * 1 / static_cast<float>(60);
			m_Position.y += leftDirection.y * 10.0f * 1 / static_cast<float>(60);
		}
		else if (Input::IsKeyPressed(GLFW_KEY_D))
		{
			m_Position.x -= leftDirection.x * 10.0f * 1 / static_cast<float>(60);
			m_Position.y -= leftDirection.y * 10.0f * 1 / static_cast<float>(60);
		}

		if (Input::IsKeyPressed(GLFW_KEY_Q))
		{
			m_Rotation += 180.0f * 1 / static_cast<float>(60);
		}
		else if (Input::IsKeyPressed(GLFW_KEY_E))
		{
			m_Rotation -= 180.0f * 1 / static_cast<float>(60);
		}

		glm::mat4 viewMatrix = glm::translate(glm::mat4(1), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		viewMatrix = glm::inverse(viewMatrix);

		m_Shader->SetMat4("u_View", viewMatrix);


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
			m_Angle += 1.0f * (1/60.0f);
		}
		else if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_2) && Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		{
			m_Angle -= 1.0f * (1 / 60.0f);
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
	}

	void Sandbox2D::OnRender()
	{
		glClearColor(m_BColor.x, m_BColor.y, m_BColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(m_VertexArray);

		for (int i = 0; i < m_Number.size(); i++)
		{
			for (int y = 0; y < m_Number2.size(); y++)
			{
				m_Shader->SetFloat3("u_Position", glm::vec3(m_Number[i] - m_Number2[y], m_Number2[y] + m_Number[i], 0.0f));
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
			}
		}
	}

	void Sandbox2D::OnImGuiRender()
	{
		Application* app = Application::Get();

		//ImGui::ShowDemoWindow();
		ImGui::Begin("Camera");
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "CAMERA CONTROLS");
		ImGui::Text("Move the camera around: WASD");
		ImGui::Text("Zoom in and out: Mouse Scroll Wheel");
		ImGui::Text("Rotate the camera: QE");
		ImGui::End();

		ImGui::Begin("Settings");
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "COLOR");
		ImGui::ColorEdit4("Background Color", reinterpret_cast<float*>(&m_BColor));
		ImGui::ColorEdit4("Circle Color", reinterpret_cast<float*>(&m_CircleColor));
		ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Aspect Ratio");
		ImGui::Text("Window Height: %f", static_cast<float>(app->GetWindow().GetHeight()));
		ImGui::Text("Wondow Width: %f", static_cast<float>(app->GetWindow().GetWidth()));
		ImGui::Text("Window Aspect Ratio: %f", m_AspectRatio);
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
		m_ZoomLevel += e.GetYOffset() * 0.05f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.30f);
		return false;
	}
}
