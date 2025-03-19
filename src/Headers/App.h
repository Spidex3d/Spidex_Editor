#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Config.h"
#include "Windows/WindowsManager.h"
#include "Windows/MainScreen.h"
#include "Shader/ShaderManager.h"
#include "Windows/Settings.h"


class App
{
public:
    App();
    ~App();

    static App* Instance();

    void Initialize(GLFWwindow* window);
    
    void RunApp();  

    void MainCamera();
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix(float aspectRatio);
           
    void Timer();

    void AppShutdown();

private:
    GLFWwindow* window;

    bool AppIsRunning = true;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

};
