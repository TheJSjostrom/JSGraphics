#pragma once
#include "Camera.h"

namespace JSG {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera);
		static void EndScene();
	private:

	};

}