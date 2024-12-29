#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw.h>
#include <imgui\imgui_impl_opengl3.h>


class MainScreen
{
public:
	static MainScreen* Instance();

	void Initialize(GLFWwindow* window);

	void SetUpImGui(GLFWwindow* window);
	void NewImguiFrame(GLFWwindow* window);
	void MainDockSpace(bool* p_open); // docking space
	void MainSceanWindow(GLFWwindow* window); // Drawing to an ImGui main Scean Window
	void MainWindowMenu(GLFWwindow* window); // main menu

	void AboutWindow(GLFWwindow* window); //About Window from the main menu

	//  ############## ImGui Main scene #######################
	void Creat_FrameBuffer();
	unsigned int LoadTextureFiles(const char* filename, GLuint* out_texture, int out_width, int out_height);

	void Bind_Framebuffer();
	void Unbinde_Frambuffer();
	void Rescale_frambuffer(float width, float height);

	//void SetViewPort(GLint x, GLint y, GLsizei width, GLsizei height);

	void RenderImGui(GLFWwindow* window); // Render the ImGui windows
	
	void ClearScreen();

private:
	GLFWwindow* window;
	GLuint FBO;
	GLuint RBO;
	GLuint main_scene_texture_id; 

	bool show_about_window = false;
	
	
};

