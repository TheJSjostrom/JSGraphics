#include "ImGuiUI/ImGuiUI.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Core/Application.h"


namespace JSG {

	ImGuiUI::ImGuiUI()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
		Application& app = *Application::Get();
		ImGui_ImplGlfw_InitForOpenGL(app.GetWindow().GetNativeWindow(), false);
		ImGui_ImplOpenGL3_Init();
	}

	ImGuiUI::~ImGuiUI()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiUI::OnRender()
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	
		ImGui::ShowDemoWindow(); 

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
	}

	void ImGuiUI::OnEvent(Event& e)
	{
		
	}
}