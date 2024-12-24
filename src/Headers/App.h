#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "Config.h"
#include "Windows/WindowsManager.h"
#include "Windows/MainScreen.h"



class App
{
public:
    App();
    ~App();

    static App* Instance();

    void Initialize(GLFWwindow* window);
    
    void RunApp();

    void AppShutdown();

private:
    GLFWwindow* window;

    bool AppIsRunning = true;

};
