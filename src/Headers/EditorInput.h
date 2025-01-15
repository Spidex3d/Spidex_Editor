#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "../Camera/Camera.h"


extern Camera camera;
extern float lastX;
extern float lastY;
extern bool firstMouse;
extern bool mouse;
extern float deltaTime;
extern float lastFrame;
extern float fov;



//void processInput(GLFWwindow* window, Camera& camera);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);




//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//float lastX = SCR_WIDTH / 2.0f;
//float lastY = SCR_HEIGHT / 2.0f;
//bool firstMouse = true;
//bool mouse = true;
//
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//float fov = 45.0f;
//
//void processInput(GLFWwindow* window, Camera& camera) // this changed
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        camera.ProcessKeyboard(FORWARD, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        camera.ProcessKeyboard(BACKWARD, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        camera.ProcessKeyboard(LEFT, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        camera.ProcessKeyboard(RIGHT, deltaTime);
//    // UP - Down
//    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
//        camera.ProcessKeyboard(UP, deltaTime);
//    }   
//    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
//        camera.ProcessKeyboard(UP, -deltaTime);
//       
//    // change widows Opacity 
//    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) 
//        glfwSetWindowOpacity(window, 0.5f);
//    
//    // change widows Opacity back to normat
//    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
//        glfwSetWindowOpacity(window, 1.0f);
//    
//}
//void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
//{
//
//    if (mouse == true) {
//
//        float xpos = static_cast<float>(xposIn);
//        float ypos = static_cast<float>(yposIn);
//
//        if (firstMouse)
//        {
//            lastX = xpos;
//            lastY = ypos;
//            firstMouse = false;
//        }
//
//        float xoffset = xpos - lastX;
//        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//
//        lastX = xpos;
//        lastY = ypos;
//
//        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_TRUE)
//            //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//            camera.ProcessMouseMovement(xoffset, yoffset);
//    }
//}
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    camera.ProcessMouseScroll(static_cast<float>(yoffset));
//}