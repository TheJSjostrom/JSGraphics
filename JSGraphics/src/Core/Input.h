#pragma once
#include <stdint.h>

namespace JSG {

	class Input
	{
	public:
		static bool IsKeyPressed(uint32_t keyCode);
		static bool IsMouseButtonPressed(uint32_t mouseCode);
	};
} 