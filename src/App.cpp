#include "Headers\App.h"
#include "Headers/EditorInput.h" // keybord input
#include "Editor_Gui/MainGui.h"
#include "Ecs\EntityComponents.h"
#include "Shader\Shader.h"

#include "Ecs\ObjectManager.h"
#include "Editor_Gui\Grid.h"
#include "Camera/Camera.h"


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

		MainScreen::Instance()->Creat_FrameBuffer(); // my frame buffer to fill main scene window
	}	

	glEnable(GL_DEPTH_TEST);


		Grid::Instance()->gridSetUp(); // Grid
		ShaderManager::SetupShaders(); // Initialize the shaders


    EntityComponents entityComponents; // this fills the object list
	entityComponents.Initialize();
		
	int currentIndex = -1;
	//int indexCube = 0, indexPlane = 0, indexSphere = 0, indexLight = 0, indexTypeID = 0;
	int index = 0, objectIndex = 0, indexTypeID = 0;
	
	// 29 lines
	while (AppIsRunning)	
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;


		// close it all down if you click the cross or press Esc button
		if (glfwWindowShouldClose(windowManager.GetWindow())) { AppIsRunning = false; }

		
		processInput(windowManager.GetWindow()); // Keybord and mouse control
		glfwPollEvents();
		
		// ########## ImGui ###########
		MainScreen::Instance()->NewImguiFrame(windowManager.GetWindow()); // New ImGui Frame 
		ImGui::NewFrame();
		
		// ############################################ GUI from Here ####################################
		MainScreen::Instance()->MainWindowMenu(windowManager.GetWindow()); // Main Menu
		MainScreen::Instance()->AboutWindow(windowManager.GetWindow());
		bool p_open = true; 
		MainScreen::Instance()->MainDockSpace(&p_open); // The Doc Space

		MainScreen::Instance()->MainSceanWindow(windowManager.GetWindow());	// Main Scene Window for drawing objects to

		EntityNode::Instance()->EntityManagmentSystem(entityComponents.GetModels(), currentIndex, index, objectIndex, indexTypeID);

		EntityNode::Instance()->EntityProperties();
		// #############################################  End GUI ##########################################


		// ############################################# Camera Object !!! ################################
		MainScreen::Instance()->Bind_Framebuffer();

		glm::mat4 model(1.0), view(1.0), projection(1.0);
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.2f, 200.0f); //0.2
	
		
		MainScreen::Instance()->ClearScreen(); // glClear - glClearColor
		
		// ############################################# Drawing Object !!! ################################
		// eventually this will need to be a renderer !!

		ShaderManager::defaultGridShader->Use();
		ShaderManager::defaultGridShader->setMat4("projection", projection);
		ShaderManager::defaultGridShader->setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f)); // this is better
		model = glm::scale(model, glm::vec3(20.0f, 0.0f, 20.0f));
		ShaderManager::defaultGridShader->setMat4("model", model);

		Grid::Instance()->gridRender();

		MainScreen::Instance()->Unbinde_Frambuffer();
				
		// ############################################# End Drawing Object !!! ################################
				
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

// Render the triangles
		/*for (const auto& model : entityComponents.GetModels()) {
			if (auto* triangle = dynamic_cast<TriangleModel*>(model.get())) {
				triangle->DrawTriangle(); }
		}*/