#pragma once

namespace JSG {

	class Sandbox2D
	{
	public:
		Sandbox2D();
		~Sandbox2D();

		void OnUpdate();
		void OnRender();
		void OnImGuiRender();

		void OnEvent();
	private:
		float m_GValue = 0.25f;
	};

}