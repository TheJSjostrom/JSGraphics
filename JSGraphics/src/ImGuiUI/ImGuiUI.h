#pragma once
#include "Events/Event.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"

namespace JSG {

	class ImGuiUI
	{
	public:
		ImGuiUI();
		~ImGuiUI();

		void OnRender();
		void OnEvent(Event& e);

		void Begin();
		void End();
	};

}