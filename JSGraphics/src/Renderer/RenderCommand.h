#pragma once
#include "glm/glm.hpp"

namespace JSG {

	class RenderCommand 
	{
	public:
		static void Init();
		static void Shutdown();

		static void SetClearColor(const glm::vec4& color);
		static void SetClearDepth(float depth);
		static void Clear();
	};

}