#include "Headers\App.h"
#include "Headers/EditorInput.h"
#include "Editor_Gui/MainGui.h"
#include "Ecs\EntityComponents.h"


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

	WindowManager windowManager(SCR_WIDTH, SCR_HEIGHT, "Spidex 3D Model Editor");
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

 
    EntityComponents entityComponents;
	entityComponents.Initialize();
		
	int currentIndex = -1;
	//int indexCube = 0, indexPlane = 0, indexSphere = 0, indexLight = 0, indexTypeID = 0;
	int index = 0, objectIndex = 0, indexTypeID = 0;
	
	// 29 lines
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
		MainScreen::Instance()->AboutWindow(windowManager.GetWindow());
		bool p_open = true; 
		MainScreen::Instance()->MainDockSpace(&p_open); // The Doc Space

		MainScreen::Instance()->MainSceanWindow(windowManager.GetWindow());	// Mani Scene Window for drawing to

		EntityNode::Instance()->EntityManagmentSystem(entityComponents.GetModels(), currentIndex, index, objectIndex, indexTypeID);
			
		for (const auto& model : entityComponents.GetModels()) {
			if (auto* triangel = dynamic_cast<TriangelModel*>(model.get())) {
				triangel->DrawTriangel();
			}
		}

		
		//indexPlane, indexSphere,  indexLight, 
		EntityNode::Instance()->EntityProperties();
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