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


	void RenderImGui(GLFWwindow* window); // Render the ImGui windows

	void ClearScreen();
	

private:
	GLFWwindow* window;

	bool show_about_window = false;
	
	
};

