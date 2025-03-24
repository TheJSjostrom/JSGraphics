#pragma once

namespace JSG {

	class Application
	{
	public:
		Application();
		~Application();

		void Run();

	private:
		bool m_Running = true;
	};

}