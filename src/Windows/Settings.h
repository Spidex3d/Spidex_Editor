#pragma once
#include <GLFW\glfw3.h>

#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw.h>
#include <imgui\imgui_impl_opengl3.h>

#include <fstream> // Include fstream for file operations


//extern bool show_settings_window;
extern bool gridNogrid;
extern float BgCol[4];
class Settings
{
public:
	
	void SettingsWindow(GLFWwindow* window);

	void SaveSettings(const std::string& filename);
	void LoadSettings(const std::string& filename);

private:
	GLFWwindow* window;
	// used for the main screen background colour
	

};

