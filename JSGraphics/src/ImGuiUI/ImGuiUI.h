#pragma once
#include "Events/Event.h"

namespace JSG {

	class ImGuiUI
	{
	public:
		ImGuiUI() = default;
		~ImGuiUI() = default;

		void Init();
		void Shutdown();

		void OnEvent(Event& e);

		void Begin();
		void End();
	};

}