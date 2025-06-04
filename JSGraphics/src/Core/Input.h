#pragma once
#include <stdint.h>

#include "glm/glm.hpp"

namespace JSG {

	class Input
	{
	public:
		static bool IsKeyPressed(uint32_t keyCode);
		static bool IsMouseButtonPressed(uint32_t mouseCode);

		static glm::vec2 GetMousePosition();
		static float GetMousePositionX();
		static float GetMousePositionY();
	};

} 