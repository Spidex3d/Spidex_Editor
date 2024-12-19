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
	void MainSceanWindow(GLFWwindow* window); // Drawing to an ImGui Scean Window
	void RenderImGui(GLFWwindow* window);

	void ClearScreen();
	void SwapBuffers();
	void PollEvents();
	void ShutDown();

	// void BgColour();


private:
	GLFWwindow* window;
};

