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




class App
{
public:
    App();
    ~App();

    static App* Instance();

    void Initialize(GLFWwindow* window);
    
    void RunApp();

    void MainCamera();

    void Timer();

    void AppShutdown();

private:
    GLFWwindow* window;

    bool AppIsRunning = true;

};
