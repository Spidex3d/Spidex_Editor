#include "ShaderManager.h"
#include <iostream>


// Define the static member
Shader * ShaderManager::defaultGridShader = nullptr; // the grid shader
Shader * ShaderManager::LightBulbShader = nullptr;   // the light bulb shader
Shader * ShaderManager::LightCubeShader = nullptr;   // the obj shaders 
Shader * ShaderManager::defaultShader = nullptr;   // the obj shaders 
Shader * ShaderManager::skyShader = nullptr;   // the sky shaders 
Shader * ShaderManager::SimpelColor = nullptr;   // the sky shaders 

glm::vec3 lightPos(1.2f, 1.5f, 2.0f);

extern glm::vec3 amb_light;
extern float deltaTime;
extern float angel;

void ShaderManager::SetupShaders() {
    // Initialize the shader
    defaultGridShader = new Shader("Shader/shaderFile/default_Grid.vert", "Shader/shaderFile/default_Grid.frag");
  
    LightBulbShader = new Shader("Shader/shaderFile/bulb.vert", "Shader/shaderFile/bulb.frag");
  
    LightCubeShader = new Shader("Shader/shaderFile/light_2.vert", "Shader/shaderFile/light_2.frag");

    defaultShader = new Shader("Shader/shaderFile/default.vert", "Shader/shaderFile/default.frag");

    skyShader = new Shader("Shader/shaderFile/sky.vert", "Shader/shaderFile/sky.frag");

    SimpelColor = new Shader("Shader/shaderFile/test.vert", "Shader/shaderFile/test.frag");
}

void ShaderManager::SetUpLightShader()
{
   

    

    //glm::vec3 lightPos(0.0f, 1.0f, 6.0f);
    ////  glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    //glm::vec3 lightColor(amb_light[0], amb_light[1], amb_light[2]);

}
