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
    defaultGridShader = new Shader("src/Shader/ShaderFile/default_Grid.vert", "src/Shader/ShaderFile/default_Grid.frag");
    
    //defaultGridShader = new Shader("Shader/ShaderFile/test.vert", "Shader/ShaderFile/test.frag");
    //defaultGridShader = new Shader("C:/Users/marty/Desktop/SpidexEditor/Spidex_Editor/src/Shader/ShaderFile/test.vert",
      //  "C:/Users/marty/Desktop/SpidexEditor/Spidex_Editor/src/Shader/ShaderFile/test.frag");
  
    /*LightBulbShader = new Shader("Shader/shaderFile/bulb.vert", "Shader/shaderFile/bulb.frag");
  
    LightCubeShader = new Shader("Shader/shaderFile/light_2.vert", "Shader/shaderFile/light_2.frag");

    defaultShader = new Shader("Shader/shaderFile/default.vert", "Shader/shaderFile/default.frag");

    skyShader = new Shader("Shader/shaderFile/sky.vert", "Shader/shaderFile/sky.frag");

    SimpelColor = new Shader("Shader/shaderFile/test.vert", "Shader/shaderFile/test.frag");*/
}


