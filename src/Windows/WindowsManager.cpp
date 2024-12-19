#include "WindowsManager.h"
#include "../SpdLog/LogInternals.h"




WindowManager::WindowManager(int width, int height, const char* title)
	: width(width), height(height), title(title), window(nullptr){}

WindowManager::~WindowManager()
{
	if (window) {
		glfwDestroyWindow(window);
	}
	glfwTerminate();
}

bool WindowManager::GLFWInitialize()
{
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        glfwTerminate();
        return false;
    }
    else {
        LogInternals::Instance()->Info("Window created successfully");
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        //std::cout << "Failed to loade glad" << std::endl;
    }
    else {
        LogInternals::Instance()->Info("Glad Loaded Correctly");
    }

   

    glViewport(0, 0, bufferWidth, bufferHeight);

    return true;
}

GLFWwindow* WindowManager::GetWindow() const
{
	return window;
}


