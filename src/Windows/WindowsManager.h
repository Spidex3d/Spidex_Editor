#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>

class WindowManager
{
public:
	// ####### This sets up the glfw window ######
	WindowManager(int width, int height, const char* title);
	~WindowManager();
	bool GLFWInitialize();
	GLFWwindow* GetWindow() const;


private:
	int width;
	int height;
	const char* title;

	GLFWwindow* window;


};

