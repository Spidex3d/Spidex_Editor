#include "Headers\App.h"
#include "Headers/EditorInput.h"


App::App()
{
}

App* App::Instance()
{
	App* mainApp = new App;
	return mainApp;
}

void App::Initialize(GLFWwindow* window)
{
	
}

void App::RunApp()
{

	WindowManager windowManager(SCR_WIDTH, SCR_HEIGHT, "Spidex 3D Editor");
	if (!windowManager.GLFWInitialize()) {
		return;
	}
	else
	{
		// Load an icon to the main form
		MainScreen::Instance()->Initialize(windowManager.GetWindow());
		// set up ImGui
		MainScreen::Instance()->SetUpImGui(windowManager.GetWindow());
	}

	while (AppIsRunning)	
	{
		// close it all down if you click the cross or press Esc button
		if (glfwWindowShouldClose(windowManager.GetWindow())) { AppIsRunning = false; }

		MainScreen::Instance()->ClearScreen(); // glClear - glClearColor
		processInput(windowManager.GetWindow()); // Keybord and mouse control
		
		// ########## ImGui ###########
		MainScreen::Instance()->NewImguiFrame(windowManager.GetWindow()); // New ImGui Frame 
		ImGui::NewFrame();
		// ########### GUI Here ##################
		MainScreen::Instance()->MainWindowMenu(windowManager.GetWindow()); // Main Menu
		bool p_open = true; 
		MainScreen::Instance()->MainDockSpace(&p_open); // The Doc Space
		ImGui::Begin("Test Window");

		ImGui::End();
		// ##########  End GUI ###################
		MainScreen::Instance()->RenderImGui(windowManager.GetWindow()); // render the imgui windows
		glfwSwapBuffers(windowManager.GetWindow()); // the last 2 lines of code
		glfwPollEvents();
	}
	App::AppShutdown();
}


void App::AppShutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	LogInternals::Instance()->Info("Engine shutdown successfully");
}

App::~App()
{
}