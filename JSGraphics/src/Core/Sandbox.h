#pragma once
#include "Events/Event.h"

namespace JSG {

	class Sandbox
	{
	public:
		virtual ~Sandbox() = default;

		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
		virtual void OnImGuiRender() = 0;
		virtual void OnEvent(Event& e) = 0;
	};

}