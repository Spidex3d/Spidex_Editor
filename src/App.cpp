#include "Headers\App.h"
#include "Ecs/EntityNodes.h"
#include "Shader\Shader.h"

#include "Ecs\ObjectManager.h"
#include "Editor_Gui\Grid.h"
#include "Camera/Camera.h"
#include "Headers\GlobalVars.h"

//#include "Ecs\Picking.h"
//#include "Ecs\Intersection.h"

#include "Headers/EditorInput.h" // keybord & mouse input

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
									
			glfwSetCursorPosCallback(windowManager.GetWindow(), mouse_callback); // this is the best place
			glfwSetScrollCallback(windowManager.GetWindow(), scroll_callback);  // set camera pos zoom

			MainScreen::Instance()->SetUpImGui(windowManager.GetWindow()); // must be set after mouse callback
		}	
				
		// ############################################# Drawing Object !!! ################################
		Grid::Instance()->gridSetUp(); // Grid
		
		ShaderManager::SetupShaders(); // Initialize the shaders
			
		MainScreen::Instance()->Creat_FrameBuffer(); // my frame buffer to fill main scene window

        //EntityComponents entityComponents; // this fills the object list
		EntityNodes entityComponents;
	    entityComponents.Initialize();
	    	
	    int currentIndex = -1;
	    //int indexCube = 0, indexPlane = 0, indexSphere = 0, indexLight = 0, indexTypeID = 0;
	    int index = 0, objectIndex = 0, indexTypeID = 0;
					    
	    glEnable(GL_DEPTH_TEST);

		Settings settings;
		settings.LoadSettings("settings.txt"); // Loading ths saved settings
		
	// 29 lines
		while (AppIsRunning)
		{
			App::Instance()->Timer();

			// close it all down if you click the cross or press Esc button
			if (glfwWindowShouldClose(windowManager.GetWindow())) { AppIsRunning = false; }
						
			processInput(windowManager.GetWindow()); // Keybord and mouse control

			// ############################################ GUI from Here ####################################
			// NewImguiFrame, MainWindowMenu, AboutWindow, MainDockSpace, MainSceanWindow
			MainScreen::Instance()->WinInit(windowManager.GetWindow()); // Initialize all the above

			MainScreen::Instance()->MainSceneWindow(windowManager.GetWindow()); // main drawing window

			settings.SettingsWindow(window);  // Initialize the Settings Window

			EntityNodes::Instance()->EntityManagmentSystem(entityComponents.GetModels(), currentIndex,
				index, objectIndex, indexTypeID); // Entity Management System Scene list

			EntityNodes::Instance()->EntityProperties(); // Property window
			
			// #############################################  End GUI ##########################################

			MainScreen::Instance()->Bind_Framebuffer(); // for the main screen

			// ############################################# Camera Object !!! ################################

			MainScreen::Instance()->BgColour(BgCol);
			MainScreen::Instance()->ClearScreen(); // glClear ready for the next frame

			App::MainCamera(); // ########## This is the main Camera ##########
						
			// Render the grid and objects
			if (!gridNogrid) {   // Show the grid or hide it
				EntityNodes::Instance()->RenderGrid(camera.GetViewMatrix(), camera.GetProjectionMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT));
		    }
			// Render the Scene and objects
			EntityNodes::Instance()->RenderScene(camera.GetViewMatrix(), camera.GetProjectionMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT), entityComponents.GetModels());
			

			EntityNodes::Instance()->ObjectEditor(entityComponents.GetModels());
				
		MainScreen::Instance()->Unbinde_Frambuffer();
				
		// ############################################# End Drawing Object !!! ################################
				
		MainScreen::Instance()->RenderImGui(windowManager.GetWindow()); // render the imgui windows	

		glfwSwapBuffers(windowManager.GetWindow()); // the last 2 lines of code
		glfwPollEvents();
	}
	App::AppShutdown(); // clean-up
}

void App::MainCamera()
{
	viewMatrix = camera.GetViewMatrix();
	projectionMatrix = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.3f, 200.0f);
}

glm::mat4 App::GetViewMatrix()
{
	return viewMatrix;
}

glm::mat4 App::GetProjectionMatrix(float aspectRatio)
{
	return projectionMatrix;
}

void App::Timer()
{
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
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

// Render the triangles
		/*for (const auto& model : entityComponents.GetModels()) {
			if (auto* triangle = dynamic_cast<TriangleModel*>(model.get())) {
				triangle->DrawTriangle(); }
		}*/

		//MainScreen::Instance()->NewImguiFrame(windowManager.GetWindow()); // New ImGui Frame 
		//ImGui::NewFrame();

		//MainScreen::Instance()->MainWindowMenu(windowManager.GetWindow()); // Main Menu
		//MainScreen::Instance()->AboutWindow(windowManager.GetWindow());
		//bool p_open = true; 
		//MainScreen::Instance()->MainDockSpace(&p_open); // The Doc Space

		//MainScreen::Instance()->MainSceanWindow(windowManager.GetWindow());	// Main Scene Window for drawing objects to
		//
		// //glm::mat4 model(1.0), view(1.0), projection(1.0);
		//view = camera.GetViewMatrix();
		//projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.2f, 200.0f); //0.2
		// 
//ShaderManager::defaultGridShader->Use();
		//ShaderManager::defaultGridShader->setMat4("projection", projection);
		//ShaderManager::defaultGridShader->setMat4("view", view);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f)); // this is better
		//model = glm::scale(model, glm::vec3(20.0f, 0.0f, 20.0f));
		//ShaderManager::defaultGridShader->setMat4("model", model);

//glm::mat4 model(1.0), view(1.0), projection(1.0);
		//view = camera.GetViewMatrix();
		//projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.2f, 200.0f); //0.2

// ############################################# Drawing Object !!! ################################

		//if (!gridNogrid) {   // Show the grid or hide it
		//	ShaderManager::defaultGridShader->Use();
		//	ShaderManager::defaultGridShader->setMat4("projection", projection);
		//	ShaderManager::defaultGridShader->setMat4("view", view);
		//	model = glm::mat4(1.0f);
		//	model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f)); // this is better
		//	model = glm::scale(model, glm::vec3(20.0f, 0.0f, 20.0f));
		//	ShaderManager::defaultGridShader->setMat4("model", model);

		//	Grid::Instance()->gridRender(); // Render the Grid
		//}			
		//
		// //############################################# Drawing Object !!! ################################
		// //eventually this will need to be a renderer !!
		//ShaderManager::defaultGridShader->Use();
		//ShaderManager::defaultGridShader->setMat4("projection", projection);
		//ShaderManager::defaultGridShader->setMat4("view", view);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // this is better
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		//ShaderManager::defaultGridShader->setMat4("model", model);
		//
		////DefaultModels::Instance()->DrawTriangle(); // render a triangle

		//for (const auto& model : entityComponents.GetModels()) {
		//	if (auto* triangle = dynamic_cast<TriangleModel*>(model.get())) {
		//		triangle->DrawTriangle(); }
		//}