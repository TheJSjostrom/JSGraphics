#pragma once
#include "Events/Event.h"

namespace JSG {

	class Sandbox
	{
	public:
		Sandbox() 
		{
		}

		virtual ~Sandbox() 
		{
		}

		virtual void OnUpdate(float ts) = 0;
		virtual void OnRender() = 0;
		virtual void OnImGuiRender() = 0;
		virtual void OnEvent(Event& e) = 0;
	};

}