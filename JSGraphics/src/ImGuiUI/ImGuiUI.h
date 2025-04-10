#pragma once
#include "Events/Event.h"

namespace JSG {

	class ImGuiUI
	{
	public:
		ImGuiUI();
		~ImGuiUI();

		void OnRender();
		void OnEvent(Event& e);
	};

}