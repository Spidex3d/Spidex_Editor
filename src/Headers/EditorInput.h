#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "App.h"

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
}