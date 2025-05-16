#include "ShaderManager.h"
#include <iostream>


// Define the static member
Shader * ShaderManager::defaultGridShader = nullptr; // the grid shader
Shader * ShaderManager::defaultGLTFShader = nullptr;
Shader * ShaderManager::LightBulbShader = nullptr;   // the light bulb shader
Shader * ShaderManager::LightCubeShader = nullptr;   // the obj shaders 
Shader * ShaderManager::defaultShader = nullptr;   // the obj shaders 
Shader * ShaderManager::skyShader = nullptr;   // the sky shaders 
Shader * ShaderManager::SimpelColor = nullptr;   // the sky shaders 
Shader * ShaderManager::TestShadre = nullptr;   // the sky shaders 

glm::vec3 lightPos(1.2f, 1.5f, 2.0f);

extern glm::vec3 amb_light;
extern float deltaTime;
extern float angel;

void ShaderManager::SetupShaders() {
    // Initialize the shader
    defaultGridShader = new Shader("src/Shader/ShaderFile/default_Grid.vert", "src/Shader/ShaderFile/default_Grid.frag");

    //defaultGLTFShader = new Shader("src/Shader/ShaderFile/gltf.vert", "src/Shader/ShaderFile/gltf.frag");
    defaultShader = new Shader("src/Shader/ShaderFile/gltf.vert", "src/Shader/ShaderFile/gltf.frag");

    //defaultShader = new Shader("src/Shader/ShaderFile/default.vert", "src/Shader/ShaderFile/default.frag");
    
    
}


