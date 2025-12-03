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
		m_Balls.reserve(20);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		/////////////////////////////////
		//////////// Ball ///////////////
		/////////////////////////////////

		glCreateVertexArrays(1, &m_BallVertexArray);
		glBindVertexArray(m_BallVertexArray);

		float BallVertices[4 * 6] = {
		//  Position            Local Position
			-0.5f, -0.5f, 0.0f, -1.0f, -1.0f, 0.0f, // Vertex Index 0
			 0.5f, -0.5f, 0.0f,  1.0f, -1.0f, 0.0f, // Vertex Index 1
			 0.5f,  0.5f, 0.0f,  1.0f,  1.0f, 0.0f, // Vertex Index 2
			-0.5f,  0.5f, 0.0f, -1.0f,  1.0f, 0.0f  // Vertex Index 3
		};

		m_BallVertexBuffer.Init(sizeof(BallVertices), BallVertices);
		m_BallVertexBuffer.Bind();

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));

		uint32_t BallIndices[6] = { 0, 1, 2, 2, 3, 0 };
		m_BallIndexBuffer.Init(sizeof(BallIndices), BallIndices);
		m_BallIndexBuffer.Bind();

		const std::string BallVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_LocalPosition;
			
			uniform mat4 u_Proj;
			uniform mat4 u_View;
			uniform mat4 u_Model;
			
			out vec3 FragPos;

			void main()
			{
				FragPos = a_LocalPosition;
				gl_Position = u_Proj * u_View * u_Model * vec4(a_Position, 1.0);
			}
		)";

		const std::string BallFragSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 FragPos;

			void main()
			{ 
				float FragPosLength = sqrt(FragPos.x * FragPos.x + FragPos.y * FragPos.y);
				color = vec4(0.0, 0.0, 0.0, 0.0);

				if (FragPosLength < 1.0)
					color = vec4(1.0, 0.0, 0.0, 1.0f);
			}
		)";

		m_BallShader.Init(BallVertexSrc, BallFragSrc);
		m_BallShader.Bind();

		/////////////////////////////////
		///////// Light Cube ////////////
		/////////////////////////////////

		glCreateVertexArrays(1, &m_LightCubeVertexArray);
		glBindVertexArray(m_LightCubeVertexArray);

		float LightCubeVertices[4 * 6] = {
		//  Position          Normal
			-0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Vertex Index 0
			 0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Vertex Index 1
			 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Vertex Index 2
			-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // Vertex Index 3
		};

		m_LightCubeVertexBuffer.Init(sizeof(LightCubeVertices), LightCubeVertices);
		m_LightCubeVertexBuffer.Bind();

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0 * sizeof(float)));

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

		// Quad
		glCreateVertexArrays(1, &m_QuadVertexArray);
		glBindVertexArray(m_QuadVertexArray);

		float QuadVertices[4 * 6] = {
     	//  Position          Normal
			-0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Vertex Index 0
			 0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Vertex Index 1
			 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Vertex Index 2
			-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // Vertex Index 3
		};

		m_QuadVertexBuffer.Init(sizeof(QuadVertices), QuadVertices);
		m_QuadVertexBuffer.Bind();

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));

		uint32_t QuadIndices[6] = { 0, 1, 2, 2, 3, 0 };
		m_QuadIndexBuffer.Init(sizeof(QuadIndices), QuadIndices);
		m_QuadIndexBuffer.Bind();

		const std::string QuadVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_Normal;

			uniform mat4 u_Proj;
			uniform mat4 u_View;
			uniform mat4 u_Model;
			
			out vec3 FragPos;
			out vec3 Normal;
			
			void main()
			{
				gl_Position = u_Proj * u_View * u_Model * vec4(a_Position, 1.0);

				FragPos = vec3(u_Model * vec4(a_Position, 1.0));
				Normal = a_Normal;
			}
		)";

		const std::string QuadFragSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform vec3 u_Color;
			uniform vec3 u_LightPos;
			uniform vec3 u_LigthDirection;
			uniform vec3 u_LightColor;
			uniform vec3 u_ObjectColor;
			uniform vec3 u_viewPos;

			in vec3 FragPos;
			in vec3 Normal;

			void main()
			{ 
				float ambientStrength = 0.001f;
				vec3 ambient = ambientStrength * u_LightColor;

				vec3 playerDir = normalize(u_LightPos - FragPos);
				float diff = max(dot(Normal, playerDir), 0.0f);
 
				vec3 diffuse = diff * u_LightColor;

				float specularStrength = 0.5;
				vec3 viewDir = normalize(u_LightPos - FragPos);
				vec3 reflectDir = reflect(-u_LigthDirection, Normal);  
				float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
				float angle = degrees(acos(diff / (length(viewDir) * length(reflectDir))));
		
				vec3 specular = specularStrength * spec * u_LightColor; 

				vec3 result = (diffuse + ambient + specular) * u_ObjectColor;
			
				color = vec4(result, 1.0);
			}
		)";

		m_QuadShader.Init(QuadVertexSrc, QuadFragSrc);
		m_QuadShader.Bind();

		// Circle
		glCreateVertexArrays(1, &m_CircleVertexArray);
		glBindVertexArray(m_CircleVertexArray);

		float CircleVertices[4 * 6] = {
		//  Position          Local Position
			-0.5f,-0.5f, 0.0f,-1.0f,-1.0f, 0.0f, // Vertex Index 0
			 0.5f,-0.5f, 0.0f, 1.0f,-1.0f, 0.0f, // Vertex Index 1
			 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // Vertex Index 2
			-0.5f, 0.5f, 0.0f,-1.0f, 1.0f, 0.0f  // Vertex Index 3
		};

		m_CircleVertexBuffer.Init(sizeof(CircleVertices), CircleVertices);
		m_CircleVertexBuffer.Bind();

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));

		// Byte Arithmetic
		// Attribute Memory Address = Base Memory Address + offset + (Vertex Index * Stride)
		// Attribute Components Memory Address = Attribute Memory Address + sizeof(type)

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
		// copy elision
		
		///////////////////////////////////
		///////// LIGHT CUBE //////////////
		///////////////////////////////////
		m_LigthCubeDirection = { glm::cos(glm::radians(m_LightCubeAngle)), glm::sin(glm::radians(m_LightCubeAngle)), 0.0f };
		m_LigthCubeDirection = glm::normalize(m_LigthCubeDirection);

		if (Input::IsKeyPressed(GLFW_KEY_T))
		{
			m_LightCubePosition.x += m_LigthCubeDirection.x * 8.0f * ts;
			m_LightCubePosition.y += m_LigthCubeDirection.y * 8.0f * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_G))
		{
			m_LightCubePosition.x -= m_LigthCubeDirection.x * 8.0f * ts;
			m_LightCubePosition.y -= m_LigthCubeDirection.y * 8.0f * ts;
		}

		if (Input::IsKeyPressed(GLFW_KEY_Y))
		{
			m_LightCubePosition.z += 8.0f * ts;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_U))
		{
			m_LightCubePosition.z -=  8.0f * ts;
		}

		m_LightCubePosition.z = glm::max(m_LightCubePosition.z, 0.0f);

		if (Input::IsKeyPressed(GLFW_KEY_F))
			m_LightCubeAngle += 180.0f * ts;
		else if (Input::IsKeyPressed(GLFW_KEY_H))
			m_LightCubeAngle -= 180.0f * ts;


		///////////////////////////////
		///////// PLAYER //////////////
		///////////////////////////////

		m_Player.OnUpdate(ts);
		
		if (m_Player.GetPosition().x >= 25.0f)
		{
			m_Player.Jump(ts); // Calls the public command method once per press
		}

		////////////////////////////////
		///////////  BALL  ////////////
		///////////////////////////////

		for (size_t i = 0; i < m_Balls.size(); i++)
		{
			m_Balls[i].OnUpdate(ts);
		}

		////////////////////////////////
		/////////// ENEMY /////////////
		///////////////////////////////

		// Geometric interpretation - Algebraic interpretation
		//         Scalar Projection
		//        |              | cos(180) = -2/2
		// ||A|| * ||B|| * cos(0) = A.x * B.x + A.y * B.y = A . B <- Dot Product
		
		m_Enemy.OnUpdate(ts, m_Player);

		////////////////////////////////
		/////////// CAMERA /////////////
		////////////////////////////////

		glm::vec3 CameraBackDirection = { 0.0f, 0.0f, 1.0f };
		glm::vec3 CameraRightDirection = glm::normalize(glm::cross(m_CameraUpDirection, CameraBackDirection));
	
		m_CameraUpDirection = { glm::cos(glm::radians(90.0f + m_CameraRotation)), glm::sin(glm::radians(90.0f + m_CameraRotation)), 0.0f };
		m_CameraUpDirection = glm::normalize(m_CameraUpDirection);

		const glm::vec3 cameraUpVelocity = m_CameraUpDirection * m_CameraSpeed * ts;
		const glm::vec3 cameraRightVelocity = CameraRightDirection * m_CameraSpeed * ts;

		if (Input::IsKeyPressed(GLFW_KEY_W)) 
		{
			m_CameraPosition += cameraUpVelocity;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_S))
		{
			m_CameraPosition -= cameraUpVelocity;
		}
		
		if (Input::IsKeyPressed(GLFW_KEY_D))
		{
			m_CameraPosition += cameraRightVelocity;
		}
		else if (Input::IsKeyPressed(GLFW_KEY_A))
		{
			m_CameraPosition -= cameraRightVelocity;
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
		glClearColor(m_BackgroundColor.x, m_BackgroundColor.y, m_BackgroundColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Camera setup.
		m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		// Entities |
		//			v

		// Render Quad Floor.
		{
			const glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), m_FloorPosition)
										* glm::rotate(glm::mat4(1.0f), glm::radians(m_FloorRotation), glm::vec3(0, 0, 1))
										* glm::scale(glm::mat4(1.0f), glm::vec3(m_FloorSize));

			m_QuadShader.Bind();
			m_QuadShader.SetMat4("u_Proj", m_Camera.GetProjectionMatrix());
			m_QuadShader.SetMat4("u_View", m_Camera.GetViewMatrix());
			m_QuadShader.SetMat4("u_Model", ModelMatrix);
			m_QuadShader.SetFloat3("u_LightPos", m_LightCubePosition);
			m_QuadShader.SetFloat3("u_LigthDirection", m_LigthCubeDirection);
			m_QuadShader.SetFloat3("u_LightColor", m_LightCubeColor);
			m_QuadShader.SetFloat3("u_ObjectColor", m_FloorColor);
			m_QuadShader.SetFloat3("viewPos", m_Camera.GetPosition());

			glBindVertexArray(m_QuadVertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
		}

		// Render Ball
		{

			m_BallShader.Bind();
			m_BallShader.SetMat4("u_Proj", m_Camera.GetProjectionMatrix());
			m_BallShader.SetMat4("u_View", m_Camera.GetViewMatrix());


			for (const auto& ball : m_Balls) 
			{
				const glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), ball.GetPosition())
											* glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f))
											* glm::scale(glm::mat4(1.0f), glm::vec3(ball.GetSize()));

				m_BallShader.SetMat4("u_Model", ModelMatrix);

				glBindVertexArray(m_BallVertexArray);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
			}
		}

		// Render quad player
		{
			glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), m_Player.GetPosition())
								  * glm::rotate(glm::mat4(1.0f), glm::radians(m_Player.GetRotation()), glm::vec3(0, 0, 1))
								  * glm::scale(glm::mat4(1.0f), glm::vec3(m_Player.GetSize()));

			if (Input::IsKeyPressed(GLFW_KEY_M))
			{
				ModelMatrix *= glm::inverse(ModelMatrix);
			}

			m_QuadShader.Bind();
			m_QuadShader.SetMat4("u_Proj", m_Camera.GetProjectionMatrix());
			m_QuadShader.SetMat4("u_View", m_Camera.GetViewMatrix());
			m_QuadShader.SetMat4("u_Model", ModelMatrix);
			m_QuadShader.SetFloat3("u_LightPos", m_LightCubePosition);
			m_QuadShader.SetFloat3("u_LigthDirection", m_LigthCubeDirection);
			m_QuadShader.SetFloat3("u_LightColor", m_LightCubeColor);
			m_QuadShader.SetFloat3("u_ObjectColor", m_Player.GetColor());
			m_QuadShader.SetFloat3("viewPos", m_Camera.GetPosition());

			glBindVertexArray(m_QuadVertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
		}

		// Render quad Enemy
		{
			const glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), m_Enemy.GetPosition())
										* glm::rotate(glm::mat4(1.0f), glm::radians(m_Enemy.GetRotation()), glm::vec3(0, 0, 1))
										* glm::scale(glm::mat4(1.0f), glm::vec3(m_Enemy.GetSize()));

			m_QuadShader.Bind();
			m_QuadShader.SetMat4("u_Proj", m_Camera.GetProjectionMatrix());
			m_QuadShader.SetMat4("u_View", m_Camera.GetViewMatrix());
			m_QuadShader.SetMat4("u_Model", ModelMatrix);
			m_QuadShader.SetFloat3("u_LightPos", m_LightCubePosition);
			m_QuadShader.SetFloat3("u_LigthDirection", m_LigthCubeDirection);
			m_QuadShader.SetFloat3("u_LightColor", m_LightCubeColor);
			m_QuadShader.SetFloat3("u_ObjectColor", m_Enemy.GetColor());
			m_QuadShader.SetFloat3("viewPos", m_Camera.GetPosition());

			glBindVertexArray(m_QuadVertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
		}

		// Render Light Cube
		{
			const glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), { m_LightCubePosition.x, m_LightCubePosition.y, 0.0 })
										* glm::rotate(glm::mat4(1.0f), glm::radians(m_LightCubeAngle), glm::vec3(0, 0, 1))
								    	* glm::scale(glm::mat4(1.0f), glm::vec3(0.25f));

			m_CircleShader.Bind();
			m_CircleShader.SetMat4("u_Proj", m_Camera.GetProjectionMatrix());
			m_CircleShader.SetMat4("u_View", m_Camera.GetViewMatrix());
			m_CircleShader.SetMat4("u_Model", ModelMatrix);
			m_CircleShader.SetFloat3("u_Color", glm::vec3(1.0f, 1.0f, 1.0f));

			glBindVertexArray(m_CircleVertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
		}
		
		// Render circles from the vector
		m_CircleShader.Bind();
		m_CircleShader.SetMat4("u_Proj", m_Camera.GetProjectionMatrix());
		m_CircleShader.SetMat4("u_View", m_Camera.GetViewMatrix());

		for (size_t i = 0; i < m_Circles.size(); i++)
		{
			const glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), m_Circles[i].GetPosition())
										* glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f))
										* glm::scale(glm::mat4(1.0f), glm::vec3(m_Circles[i].GetSize()));

			m_CircleShader.SetMat4("u_Model", ModelMatrix);
			m_CircleShader.SetFloat3("u_Color", m_Circles[i].GetColor());

			glBindVertexArray(m_CircleVertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
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
		// CAMERA
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

		// PLAYER
		ImGui::Begin("PLAYER");
		ImGui::TextColored(ImVec4(0.941f, 1.0f, 0.0f, 1.0f), "CONTROLS");
		ImGui::Text("Move forward & backward: Up & Down arrow keys.");
		ImGui::Text("Change Direction: Left & Right arrow key.");
		ImGui::Text(" ");
		ImGui::TextColored(ImVec4(0.941f, 1.0f, 0.0f, 1.0f), "INFORMATION");
		ImGui::Text("Rotation Angle: %f", m_Player.GetRotation());
		ImGui::Text("Size: %f", m_Player.GetSize());
		ImGui::Text("Position: { %f, %f }", m_Player.GetPosition().x, m_Player.GetPosition().y);
		ImGui::End();
		
		// ENEMY
		ImGui::Begin("ENEMY");
		ImGui::TextColored(ImVec4(0.941f, 1.0f, 0.0f, 1.0f), "CONTROLS");
		ImGui::Text("Move forward & backward: O & L keys.");
		ImGui::Text("Change Direction: J & L keys.");
		ImGui::Text(" ");
		ImGui::TextColored(ImVec4(0.941f, 1.0f, 0.0f, 1.0f), "INFORMATION");
		ImGui::Text("Rotation Angle: %f", m_Enemy.GetRotation());
		ImGui::Text("Size: %f", m_Enemy.GetSize());
		ImGui::Text("Position: { %f, %f }", m_Enemy.GetPosition().x, m_Enemy.GetPosition().y);
		ImGui::End();

		// Window
		ImGui::Begin("WINDOW");
		ImGui::TextColored(ImVec4(0.941f, 1.0f, 0.0f, 1.0f), "INFORMATION");
		ImGui::Text("Window Height: %f", static_cast<float>(app->GetWindow().GetHeight()));
		ImGui::Text("Wondow Width: %f", static_cast<float>(app->GetWindow().GetWidth()));
		ImGui::Text("Window Aspect Ratio: %f", m_AspectRatio);
		ImGui::End();

		////////////////////////////////////
		/////////// EDIT TOOL /////////////
		//////////////////////////////////
		ImGui::Begin("Edit Tool");
		ImGui::TextColored(ImVec4(0.941f, 1.0f, 0.0f, 1.0f), "COLOR");
		ImGui::ColorEdit4("Background Color", reinterpret_cast<float*>(&m_BackgroundColor));

		ImGui::Text("");

		ImGui::TextColored(ImVec4(0.941f, 1.0f, 0.0f, 1.0f), "Camera Settings");
		if (ImGui::Button("Set Angle to 0."))
			m_CameraRotation = 0.0f;

		if (ImGui::Button("Set camera position to { 0.0, 0.0, 0.0 }"))
			m_CameraPosition = { 0.0f, 0.0f, 0.0f };

		ImGui::SliderFloat("Size", &m_VCircleSize, 0.1f, 10.0f);
		ImGui::ColorEdit4("VCircle Color", reinterpret_cast<float*>(&m_VColor));
	    ImGui::Text("Number of circles: %d", static_cast<int>(m_Circles.size()));
		if (ImGui::Button("Clear Circles."))
		{
			m_Circles.clear();
		}
		ImGui::Text("");
		
		// Floor Settings 
		ImGui::TextColored(ImVec4(0.941f, 1.0f, 0.0f, 1.0f), "Floor Settings");
		ImGui::ColorEdit4("Floor Color", reinterpret_cast<float*>(&m_FloorColor));
		ImGui::Text(" ");

		// Player Settings
		ImGui::TextColored(ImVec4(0.941f, 1.0f, 0.0f, 1.0f), "Player Settings");
		ImGui::ColorEdit4("Player Color", reinterpret_cast<float*>(&m_Player.GetColor()));
		ImGui::Text(" ");
		ImGui::End();
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
		// Assume you have access to your window width/height, view matrix, and projection matrix
		int windowWidth = Application::Get()->GetWindow().GetWidth();  // Replace with your actual window width variable
		int windowHeight = Application::Get()->GetWindow().GetHeight(); // Replace with your actual window height variable

		// Get mouse position (assuming you store it as member variables or get it via function)
		double mouseX = Input::GetMousePositionX(); // Example of how you might get the current mouse X
		double mouseY = Input::GetMousePositionY(); // Example of how you might get the current mouse Y

		// Define the screen position vector
		// OpenGL expects the Y origin to be bottom-left, but GLFW reports Y from top-left.
		// So we must flip the Y coordinate:
		glm::vec3 winCoords(mouseX, windowHeight - mouseY, 0.0f); // Z is 0 for the near plane

		// Define the viewport
		glm::vec4 viewport(0.0f, 0.0f, (float)windowWidth, (float)windowHeight);

		// Get your current matrices (replace with actual calls to your camera/renderer class)
		glm::mat4 viewMatrix = m_Camera.GetViewMatrix();
		glm::mat4 projectionMatrix = m_Camera.GetProjectionMatrix();

		// Unproject the coordinates
		glm::vec3 worldPosition = glm::unProject(
			winCoords,
			viewMatrix,
			projectionMatrix,
			viewport
		);

		// Now use the calculated worldPosition for your ball
		// Note: If you are working in a 2D environment, you might need to adjust the Z coordinate 
		// of the resulting worldPosition to match your game plane (e.g., set worldPosition.z = 0.0f).
		m_Balls.emplace_back(worldPosition, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
		return false;
		
	//	if (GLFW_MOUSE_BUTTON_2 == e.GetMouseButton() && m_Circles.size() <= 20)
	//		m_Balls.emplace_back(m_Camera.GetPosition(), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
	
	//	return false;
	}

}
