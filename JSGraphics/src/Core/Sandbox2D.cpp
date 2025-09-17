#include "Core/Sandbox2D.h"
#include "Core/Window.h"
#include "Core/Application.h"
#include "Core/Core.h"
#include "Core/Input.h"

#include "Events/MouseEvent.h"

#include "imgui.h"

#include "glad/glad.h"

#include "glm/gtc/matrix_transform.hpp"
#include <algorithm>
#include <print>
#include <iostream>

namespace JSG {

	Sandbox2D::Sandbox2D() : 
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		/////////////////////////////////
		///////// Light Cube ////////////
		/////////////////////////////////

		glCreateVertexArrays(1, &m_LightCubeVertexArray);
		glBindVertexArray(m_LightCubeVertexArray);

		float LightCubeVertices[4 * 3] = {
			// Vertex Position
			   -0.5f, -0.5f, 0.0f,
				0.5f, -0.5f, 0.0f,
				0.5f,  0.5f, 0.0f,
			   -0.5f,  0.5f, 0.0f
		};

		m_LightCubeVertexBuffer.Init(sizeof(LightCubeVertices), LightCubeVertices);
		m_LightCubeVertexBuffer.Bind();

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		uint32_t LightCubeIndices[6] = { 0, 1, 2, 2, 3, 0 };
		m_LightCubeIndexBuffer.Init(sizeof(LightCubeIndices), LightCubeIndices);
		m_LightCubeIndexBuffer.Bind();

		const std::string LightCubeVertexSrc = R"(
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

		const std::string LightCubeFragSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			void main()
			{ 
				color = vec4(1.0);
			}
		)";

		m_LightCubeShader.Init(LightCubeVertexSrc, LightCubeFragSrc);
		m_LightCubeShader.Bind();

		// Triangle
		glCreateVertexArrays(1, &m_TriangleVertexArray);
		glBindVertexArray(m_TriangleVertexArray);

		float TriangleVertices[3 * 3] = {
			// Vertex Position
			-0.5f, 0.0f, 0.0f,
			 0.5f, 0.0f, 0.0f,
			 0.0f, 0.5f, 0.0
		};

		m_TriangleVertexBuffer.Init(sizeof(TriangleVertices), TriangleVertices);
		m_TriangleVertexBuffer.Bind();

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		uint32_t TriangleIndices[3] = { 0, 1, 2 };
		m_TriangleIndexBuffer.Init(sizeof(TriangleIndices), TriangleIndices);
		m_TriangleIndexBuffer.Bind();

		const std::string TriangleVertexSrc = R"(
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

		const std::string TriangleFragSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform vec3 u_Color;

			void main()
			{ 
				color = vec4(u_Color, 1.0);
			}
		)";

		m_TriangleShader.Init(TriangleVertexSrc, TriangleFragSrc);
		m_TriangleShader.Bind();

		// Quad
		glCreateVertexArrays(1, &m_QuadVertexArray);
		glBindVertexArray(m_QuadVertexArray);

		float QuadVertices[4 * 3] = {
     	// Vertex Position
		   -0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f, 
			0.5f,  0.5f, 0.0f,
		   -0.5f,  0.5f, 0.0f
		};

		m_QuadVertexBuffer.Init(sizeof(QuadVertices), QuadVertices);
		m_QuadVertexBuffer.Bind();

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		
		uint32_t QuadIndices[6] = { 0, 1, 2, 2, 3, 0 };
		m_QuadIndexBuffer.Init(sizeof(QuadIndices), QuadIndices);
		m_QuadIndexBuffer.Bind();

		const std::string QuadVertexSrc = R"(
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

		const std::string QuadFragSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform vec3 u_Color;

			void main()
			{ 
				color = vec4(u_Color, 1.0);
			}
		)";

		m_QuadShader.Init(QuadVertexSrc, QuadFragSrc);
		m_QuadShader.Bind();

		// Circle
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
		Application& app = *Application::Get();
		m_AspectRatio = app.GetWindow().GetWidth() / static_cast<float>(app.GetWindow().GetHeight());
	
		// Player
		//glm::vec3 PlayerUpDirection = { 0.0f, 0.0f, 1.0f };
		//glm::vec3 PlayerLeftDirection = glm::normalize(glm::cross(PlayerUpDirection, m_PlayerForwardDirection));
		
		// cos(v) = X koordinat
		// sin(v) = Y koordinat
		m_Vector = { glm::cos(glm::radians(m_VectorRotation)), glm::sin(glm::radians(m_VectorRotation)), 0.0f };

		///////////////////////////////////
		///////// LIGHT CUBE //////////////
		///////////////////////////////////
		
		m_LigthDirection = { glm::cos(glm::radians(m_LightCubeAngle)), glm::sin(glm::radians(m_LightCubeAngle)), 0.0f };
		m_LigthDirection = glm::normalize(m_LigthDirection);

		m_LightPosition = { -3.0f, glm::sin(glm::radians(m_LightCubeAngle)) * 5.0f, 0.0f };
		m_LightCubeAngle += 180.0f * ts;

		/*
		if (Input::IsKeyPressed(GLFW_KEY_F))
			m_LightCubeAngle += 180.0f * ts;
		else if (Input::IsKeyPressed(GLFW_KEY_H))
			m_LightCubeAngle -= 180.0f * ts;
		*/

		///////////////////////////////
		///////// PLAYER //////////////
		///////////////////////////////

		m_PlayerForwardDirection = { glm::cos(glm::radians(m_PlayerRotation)), glm::sin(glm::radians(m_PlayerRotation)), 0.0f };
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

		// PLAYER ROTATION
		if (Input::IsKeyPressed(GLFW_KEY_LEFT))
		{
			m_PlayerRotation += m_PlayerRotationVelocity * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_RIGHT))
		{
			m_PlayerRotation -= m_PlayerRotationVelocity * ts;
		}

		// PLAYER SIZE
		if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		{
			m_PlayerSize += m_PlayerSizeVelocity * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_X))
		{
			m_PlayerSize -= m_PlayerSizeVelocity * ts;
		}

		m_PlayerSize = std::max(m_PlayerSize, 1.0f);
	
		////////////////////////////////
		/////////// ENEMY /////////////
		///////////////////////////////

		if (Input::IsKeyPressed(GLFW_KEY_F))
		{
			m_EnemyRotation += 180.0f * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_G))
		{
			m_EnemyRotation -= 180.0f * ts;
		}

		// Geometric interpretation - Algebraic interpretation
		//         Scalar Projection
		//        |              | cos(180) = -2/2
		// ||A|| * ||B|| * cos(0) = A.x * B.x + A.y * B.y = A . B <- Dot Product

		m_EnemyForwardDirection = { glm::cos(glm::radians(m_EnemyRotation)), glm::sin(glm::radians(m_EnemyRotation)), 0.0f };
		m_EnemyForwardDirection = glm::normalize(m_EnemyForwardDirection);

		const glm::vec3 positionDiff = glm::normalize(m_PlayerPosition - m_EnemyPosition);
		const float dotProduct = glm::dot(m_EnemyForwardDirection, positionDiff);
		m_Angle = glm::degrees(glm::acos(dotProduct / (glm::length(positionDiff) * glm::length(m_EnemyForwardDirection))));
		
		if (m_Angle <= m_EnemyFOVAngle)
		{
			m_PlayerColor = { 1.0f, 0.0f, 0.0f };
		}
		else if (m_Angle > m_EnemyFOVAngle)
		{
			m_PlayerColor = { 0.0f, 1.0f, 0.0f };
		}
 
		////////////////////////////////
		/////////// CAMERA /////////////
		////////////////////////////////

		glm::vec3 CameraBackwardDirection = { 0.0f, 0.0f, 1.0f };
		glm::vec3 CameraRightDirection = glm::normalize(glm::cross(m_CameraUpDirection, CameraBackwardDirection));
	
		m_CameraUpDirection = { glm::cos(glm::radians(m_CameraRotation + 90.0f)), glm::sin(glm::radians(m_CameraRotation + 90.0f)), 0.0f };
		m_CameraUpDirection = glm::normalize(m_CameraUpDirection);

		if (Input::IsKeyPressed(GLFW_KEY_W)) 
		{
			m_CameraPosition.x += m_CameraUpDirection.x * m_CameraVelocity * ts;
			m_CameraPosition.y += m_CameraUpDirection.y * m_CameraVelocity * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_S))
		{
			m_CameraPosition.x -= m_CameraUpDirection.x * m_CameraVelocity * ts;
			m_CameraPosition.y -= m_CameraUpDirection.y * m_CameraVelocity * ts;
		}
		
		if (Input::IsKeyPressed(GLFW_KEY_D))
		{
			m_CameraPosition.x += CameraRightDirection.x * m_CameraVelocity * ts;
			m_CameraPosition.y += CameraRightDirection.y * m_CameraVelocity * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_A))
		{
			m_CameraPosition.x -= CameraRightDirection.x * m_CameraVelocity * ts;
			m_CameraPosition.y -= CameraRightDirection.y * m_CameraVelocity * ts;
		}

		if (Input::IsKeyPressed(GLFW_KEY_Q))
		{
			m_CameraRotation += 180.0f * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_E))
		{
			m_CameraRotation -= 180.0f * ts;
		}

		///////////////////////////////////////
		////////////// DOT PRODUCT ////////////
		///////////////////////////////////////

		float ambientStrength = 0.05f;
		glm::vec3 ambient = ambientStrength * m_LightColor;
		
		glm::vec3 playerPosDir = glm::normalize(m_PlayerPosition - m_EnemyPosition);
		float diff = glm::max(glm::dot(m_LigthDirection, playerPosDir), 0.0f);
		glm::vec3 diffuse = diff * m_LightColor;
		
		m_Result = (diffuse + ambient) * m_PlayerColor;

		// Scaling Matrix

		// Rotation Matrix
		// [cos(45.0f) -sin(45.0f)]  {0.5}
		// [sin(45.0f)  cos(45.0f)]  {0.5}
		//
		// 0.5 * {cos(45.0f)} + 0.5 * {-sin(45.0f)}
		//       {sin(45.0f)}         { cos(45.0f)}
    	//	
		// {0.5 * cos(45.0f)} + {0.5 * -sin(45.0f)}
		// {0.5 * sin(45.0f)}   {0.5 *  cos(45.0f)}
		// 
		// Translation Matrix.
		// [1 0 0 2]  {0.5}        {1}       {0}     {0}   {2}       {0.5}   {2} 
		// [0 1 0 3]  {0.5}	   0.5*{0} + 0.5*{1} + 0*{0} + {3}       {0.5} + {3} 
		// [0 0 1 0]  {0}          {0}       {0}     {1}   {0}       {0}     {0}
		
	}

	void Sandbox2D::OnRender()
	{
		// Clear the the whole scene.
		glClearColor(m_BColor.x, m_BColor.y, m_BColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		/*
		const glm::mat4 projectionMatrix = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -1.0f, 1.0f);
		glm::mat4 viewMatrix = glm::translate(glm::mat4(1), m_CameraPosition) 
							 * glm::rotate(glm::mat4(1.0f), glm::radians(m_CameraRotation), glm::vec3(0, 0, 1));
		viewMatrix = glm::inverse(viewMatrix);
		*/

		// Entities |
		//			v
		// Render Light Cube
		{
			const glm::mat4 modelMatrix = glm::translate(glm::mat4(1), m_LightPosition)
										* glm::rotate(glm::mat4(1.0f), glm::radians(m_LightCubeAngle), glm::vec3(0, 0, 1))
										* glm::scale(glm::mat4(1), glm::vec3(0.5f));

			m_LightCubeShader.Bind();
			m_LightCubeShader.SetMat4("u_Proj", m_Camera.GetProjectionMatrix());
			m_LightCubeShader.SetMat4("u_View", m_Camera.GetViewMatrix());
			m_LightCubeShader.SetMat4("u_Model", modelMatrix);
			//m_LightCubeShader.SetFloat3("u_Color", m_Result);

			glBindVertexArray(m_LightCubeVertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}
		
		// Render quad player
		{
			const glm::mat4 modelMatrix = glm::translate(glm::mat4(1), m_PlayerPosition)
										* glm::rotate(glm::mat4(1.0f), glm::radians(m_PlayerRotation), glm::vec3(0, 0, 1))
										* glm::scale(glm::mat4(1), glm::vec3(m_PlayerSize));

			m_QuadShader.Bind();
			m_QuadShader.SetMat4("u_Proj", m_Camera.GetProjectionMatrix());
			m_QuadShader.SetMat4("u_View", m_Camera.GetViewMatrix());
			m_QuadShader.SetMat4("u_Model", modelMatrix);
			m_QuadShader.SetFloat3("u_Color", m_Result);

			glBindVertexArray(m_QuadVertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}
		
		// Render quad Enemy
		{
			const glm::mat4 modelMatrix = glm::translate(glm::mat4(1), m_EnemyPosition)
										* glm::rotate(glm::mat4(1.0f), glm::radians(m_EnemyRotation), glm::vec3(0, 0, 1))
										* glm::scale(glm::mat4(1), glm::vec3(m_EnemySize));

			m_QuadShader.Bind();
			m_QuadShader.SetMat4("u_Proj", m_Camera.GetProjectionMatrix());
			m_QuadShader.SetMat4("u_View", m_Camera.GetViewMatrix());
			m_QuadShader.SetMat4("u_Model", modelMatrix);
			m_QuadShader.SetFloat3("u_Color", m_EnemyColor);

			glBindVertexArray(m_QuadVertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}

		// Render a triangle
		{
			const glm::mat4 modelMatrix = glm::translate(glm::mat4(1), m_TrianglePosition)
										* glm::rotate(glm::mat4(1.0f), glm::radians(m_TriangleRotation), glm::vec3(0, 0, 1))
			                            * glm::scale(glm::mat4(1), glm::vec3(m_TriangleSize));

			m_TriangleShader.Bind();
			m_TriangleShader.SetMat4("u_Proj", m_Camera.GetProjectionMatrix());
			m_TriangleShader.SetMat4("u_View", m_Camera.GetViewMatrix());
			m_TriangleShader.SetMat4("u_Model", modelMatrix);
			m_TriangleShader.SetFloat3("u_Color", m_TriangleColor);

			glBindVertexArray(m_TriangleVertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
		}

		// Render a quad.
		{
			const glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_QuadPosition)
			    	                    * glm::rotate(glm::mat4(1.0f), glm::radians(m_QuadRotation), glm::vec3(0.0f, 0.0f, 1.0f))
				                        * glm::scale(glm::mat4(1.0f), glm::vec3(m_QuadSize));

			m_QuadShader.Bind();
			m_QuadShader.SetMat4("u_Proj", m_Camera.GetProjectionMatrix());
			m_QuadShader.SetMat4("u_View", m_Camera.GetViewMatrix());
			m_QuadShader.SetMat4("u_Model", modelMatrix);
			m_CircleShader.SetFloat3("u_Color", m_QuadColor);

			glBindVertexArray(m_QuadVertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}

		// Render a circle.
		{
			const glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), m_CirclePosition)
								        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f))
							            * glm::scale(glm::mat4(1.0f), glm::vec3(m_CircleSize));

			m_CircleShader.Bind();
			m_CircleShader.SetMat4("u_Proj", m_Camera.GetProjectionMatrix());
			m_CircleShader.SetMat4("u_View", m_Camera.GetViewMatrix());
			m_CircleShader.SetMat4("u_Model", modelMatrix);
			m_CircleShader.SetFloat3("u_Color", m_CircleColor);

			glBindVertexArray(m_CircleVertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}
		
		// Render circles from the vector
		m_CircleShader.Bind();
		m_CircleShader.SetMat4("u_Proj", m_Camera.GetProjectionMatrix());
		m_CircleShader.SetMat4("u_View", m_Camera.GetViewMatrix());

		for (size_t i = 0; i < m_Circles.size(); i++)
		{
			const glm::mat4 modelMatrix = glm::translate(glm::mat4(1), m_Circles[i].GetPosition())
										* glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f))
										* glm::scale(glm::mat4(1), glm::vec3(m_Circles[i].GetSize()));

			m_CircleShader.SetMat4("u_Model", modelMatrix);
			m_CircleShader.SetFloat3("u_Color", m_Circles[i].GetColor());

			glBindVertexArray(m_CircleVertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}


		// Render a quad made of circles
		{
			m_CircleShader.Bind();
			m_CircleShader.SetMat4("u_Proj", m_Camera.GetProjectionMatrix());
			m_CircleShader.SetMat4("u_View", m_Camera.GetViewMatrix());

			for (uint32_t i = 0; i < 20; i++)
			{
				for (uint32_t y = 0; y < 20; y++)
				{
					const glm::mat4 modelMatrix = glm::translate(glm::mat4(1), {i, y, 0.0f}) 
										        * glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f)) 
										        * glm::scale(glm::mat4(1), glm::vec3(0.5f, 0.5f, 0.5f));
					m_CircleShader.SetMat4("u_Model", modelMatrix);
					m_CircleShader.SetFloat3("u_Color", { m_QCColor, 0.0f, 0.1f * i});

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

		// TODO: UPPDATERA EDIT TOOL!

		//V1 info
		ImGui::Begin("V1");
		ImGui::SliderFloat("Rotation slider", &m_VectorRotation, 0.0f, 360.0f);
		ImGui::Text("X-coord: cos(%f) = %f", m_VectorRotation, m_Vector.x);
		ImGui::Text("Y-coord: sin(%f) = %f", m_VectorRotation, m_Vector.y);
		ImGui::End();

		// Camera
		ImGui::Begin("CAMERA");
		ImGui::TextColored(ImVec4(0.941f, 1.0f, 0.0f, 1.0f), "CONTROLS");
		ImGui::Text("Move around: WASD");
		ImGui::Text("Zoom in and out: Mouse Scroll Wheel");
		ImGui::Text("Rotate: QE");
		ImGui::Text(" ");
		ImGui::TextColored(ImVec4(0.941f, 1.0f, 0.0f, 1.0f), "INFORMATION");
		ImGui::Text("Rotation Angle: %f", m_CameraRotation);
		ImGui::Text("Zoom Level: %f", m_ZoomLevel);
		ImGui::Text("Position: { %f, %f }", m_CameraPosition.x, m_CameraPosition.y);
		ImGui::End();

		// Player
		ImGui::Begin("PLAYER");
		ImGui::TextColored(ImVec4(0.941f, 1.0f, 0.0f, 1.0f), "CONTROLS");
		ImGui::Text("Move forward & backward: Up & Down arrow keys.");
		ImGui::Text("Change Direction: Left & Right arrow key.");
		ImGui::Text(" ");
		ImGui::TextColored(ImVec4(0.941f, 1.0f, 0.0f, 1.0f), "INFORMATION");
		ImGui::Text("Rotation Angle: %f", m_PlayerRotation);
		ImGui::Text("Size: %f", m_PlayerSize);
		ImGui::Text("Speed: %funits/s", m_PlayerVelocity*60.0f);
		ImGui::Text("Position: { %f, %f }", m_PlayerPosition.x, m_PlayerPosition.y);
		ImGui::End();
		
		// Window
		ImGui::Begin("WINDOW");
		ImGui::TextColored(ImVec4(0.941f, 1.0f, 0.0f, 1.0f), "INFORMATION");
		ImGui::Text("Window Height: %f", static_cast<float>(app->GetWindow().GetHeight()));
		ImGui::Text("Wondow Width: %f", static_cast<float>(app->GetWindow().GetWidth()));
		ImGui::Text("Window Aspect Ratio: %f", m_AspectRatio);
		ImGui::End();

		// Edit tool
		ImGui::Begin("Edit Tool");
		ImGui::TextColored(ImVec4(0.941f, 1.0f, 0.0f, 1.0f), "COLOR");
		ImGui::ColorEdit4("Background Color", reinterpret_cast<float*>(&m_BColor));
		ImGui::SliderFloat("Circles Color", &m_QCColor, 0.0f, 1.0f);
		ImGui::Text("");

		ImGui::TextColored(ImVec4(0.941f, 1.0f, 0.0f, 1.0f), "Camera Settings");
			if (ImGui::Button("Set Angle to 0."))
				m_CameraRotation = 0.0f;

		ImGui::SliderFloat("Size", &m_VCircleSize, 0.5f, 10.0f);
		ImGui::ColorEdit4("VCircle Color", reinterpret_cast<float*>(&m_VColor));
	    ImGui::Text("Number of circles: %d", static_cast<int>(m_Circles.size()));
		if (ImGui::Button("Clear Circles."))
		{
			m_Circles.clear();
		}
		ImGui::Text("");

		// Player Settings
		ImGui::TextColored(ImVec4(0.941f, 1.0f, 0.0f, 1.0f), "Player Settings");
		ImGui::ColorEdit4("Player Color", reinterpret_cast<float*>(&m_PlayerColor));
		ImGui::Text(" ");

		// Circle Settings
		ImGui::TextColored(ImVec4(0.941f, 1.0f, 0.0f, 1.0f), "Circle Settings");
		ImGui::SliderFloat("Circle Size", &m_CircleSize, 1.0f, 100.0f);
		ImGui::SliderFloat("Circle X Position", &m_CirclePosition.x, -100.0f, 100.0f);
		ImGui::SliderFloat("Circle Y Position", &m_CirclePosition.y, -100.0f, 100.0f);
		ImGui::ColorEdit4("Circle Color", reinterpret_cast<float*>(&m_CircleColor));
		ImGui::Text(" ");

		// Quad Settings
		ImGui::TextColored(ImVec4(0.941f, 1.0f, 0.0f, 1.0f), "Quad Settings");
		ImGui::SliderFloat("Quad Size", &m_QuadSize, 1.0f, 100.0f);
		ImGui::SliderFloat("Quad Rotation", &m_QuadRotation, 0.0f, 360.0f);
		ImGui::SliderFloat("Quad X Position", &m_QuadPosition.x, -100.0f, 100.0f);
		ImGui::SliderFloat("Quad Y Position", &m_QuadPosition.y, -100.0f, 100.0f);
		ImGui::ColorEdit4("Quad Color", reinterpret_cast<float*>(&m_QuadColor));
		ImGui::Text(" ");

		// Triangle Settings
		ImGui::TextColored(ImVec4(0.941f, 1.0f, 0.0f, 1.0f), "Triangle Settings");
		ImGui::SliderFloat("Triangle Size", &m_TriangleSize, 1.0f, 100.0f);
		ImGui::SliderFloat("Triangle Rotation", &m_TriangleRotation, 0.0f, 360.0f);
		ImGui::SliderFloat("Triangle X Position", &m_TrianglePosition.x, -100.0f, 100.0f);
		ImGui::SliderFloat("Triangle Y Position", &m_TrianglePosition.y, -100.0f, 100.0f);
		ImGui::ColorEdit4("Triangle Color", reinterpret_cast<float*>(&m_TriangleColor));

		ImGui::End();

		/*
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
		*/
	}

	void Sandbox2D::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(Sandbox2D::OnMouseScrolled));
		dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(Sandbox2D::OnMouseButtonPressed));
	}

	bool Sandbox2D::OnMouseScrolled(const MouseScrolledEvent& e)
	{
		m_ZoomLevel += e.GetYOffset() * 0.5f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.50f);
		return false;
	}

	bool Sandbox2D::OnMouseButtonPressed(const MouseButtonPressedEvent& e)
	{
		if (GLFW_MOUSE_BUTTON_2 == e.GetMouseButton())
			m_Circles.emplace_back(Circle(m_VCircleSize, m_Camera.GetPosition(), m_VColor));
		
		return false;
	}

}
