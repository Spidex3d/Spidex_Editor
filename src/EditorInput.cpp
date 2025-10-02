#include "Headers\EditorInput.h"
#include "Headers\Config.h"
#include "Headers\GlobalVars.h"
#include "Ecs/EntityNodes.h"
#include "Ecs/Player.h"

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool mouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float fov = 45.0f;

// ############## New ######################

void processInput(GLFWwindow* window) // this changed
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    // UP - Down
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, -deltaTime);

    // change widows Opacity 
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        glfwSetWindowOpacity(window, 0.5f);

    // change widows Opacity back to normat
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        glfwSetWindowOpacity(window, 1.0f);

}

//void processInput(GLFWwindow* window, PlayerModel& player)
//{   
//
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//
//    float speed = 8.0f;
//    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) speed *= 2.0f;
//
//    // movement in player's local space
//    glm::vec3 f = player.Forward();
//    glm::vec3 r = player.RightAxis();
//    glm::vec3 u = player.UpAxis();
//
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) player.position += f * speed * deltaTime;
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) player.position -= f * speed * deltaTime;
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) player.position -= r * speed * deltaTime;
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) player.position += r * speed * deltaTime;
//
//    // Up/Down thrusters (kept from your camera controls)
//    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) player.position += u * speed * deltaTime;
//    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) player.position -= u * speed * deltaTime;
//
//    // Optional roll (space-ship feel)
//    float rollSpeed = 60.0f; // deg/sec
//    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) player.roll += rollSpeed * deltaTime;
//    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) player.roll -= rollSpeed * deltaTime;
//
//    // Window opacity (unchanged)
//    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) glfwSetWindowOpacity(window, 0.5f);
//    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) glfwSetWindowOpacity(window, 1.0f);
//
//    // Rebuild model matrix after movement/roll
//    player.UpdateModelMatrix();
//}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{

    if (mouse == true) {

        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_TRUE)
            //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            camera.ProcessMouseMovement(xoffset, yoffset);
    }
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}



// #####################################################
//static bool freeLook = false;

//void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
//{
//    float xpos = static_cast<float>(xposIn);
//    float ypos = static_cast<float>(yposIn);
//
//    if (firstMouse) { lastX = xpos; lastY = ypos; firstMouse = false; }
//
//    float xoffset = xpos - lastX;
//    float yoffset = lastY - ypos;  // y reversed
//    lastX = xpos;
//    lastY = ypos;
//
//    // Free look if RMB is held: rotate camera only (doesn't steer the ship)
//    freeLook = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
//    if (freeLook) {
//        camera.ProcessMouseMovement(xoffset, yoffset);
//        return;
//    }
//    EntityNodes entityComponents;
//    entityComponents.Initialize();
//    // Otherwise steer the player with the mouse
//    // Acquire your player instance however you store it:
//    auto* player = dynamic_cast<PlayerModel*>(entityComponents.GetModels()[PlayerIdx].get());
//    if (!player) return;
//
//    const float sens = 0.1f; // match your camera sensitivity if you like
//    player->yaw += xoffset * sens;
//    player->pitch += yoffset * sens;
//    player->pitch = glm::clamp(player->pitch, -89.0f, 89.0f);
//    player->UpdateModelMatrix();
//}
//
//void scroll_callback(GLFWwindow* window, double /*xoffset*/, double yoffset)
//{
//    camera.ProcessMouseScroll(static_cast<float>(yoffset)); // keep this for FOV zoom
//}
// ############## End New ######################

//void processInput(GLFWwindow* window, Camera& camera) // this changed
//void processInput(GLFWwindow* window) // this changed
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//
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


