#pragma once

namespace JSG {

	class ImGuiUI
	{
	public:
		static void Init();
		static void Shutdown();

		static void Begin();
		static void End();

		static void SetDarkThemeColors();
	};

}