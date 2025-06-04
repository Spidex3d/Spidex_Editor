#include "ShaderManager.h"
#include <iostream>


// Define the static member
Shader * ShaderManager::defaultGridShader = nullptr; // the grid shader
Shader * ShaderManager::defaultGLTFShader = nullptr;
Shader * ShaderManager::LightBulbShader = nullptr;   // the light bulb shader
Shader * ShaderManager::LightShader = nullptr;   // the obj shaders 
Shader * ShaderManager::defaultShader = nullptr;   // the obj shaders 
Shader * ShaderManager::skyShader = nullptr;   // the sky shaders 
Shader * ShaderManager::TerrainShader = nullptr;   // the sky shaders 
Shader * ShaderManager::TestShader = nullptr;   // the sky shaders 

glm::vec3 lightPos(1.2f, 1.5f, 2.0f);

extern glm::vec3 amb_light;
extern float deltaTime;
extern float angel;

    // Initialize the shader
void ShaderManager::SetupShaders() {
    // This is the grid shader
    defaultGridShader = new Shader("src/Shader/ShaderFile/default_Grid.vert", "src/Shader/ShaderFile/default_Grid.frag");

    //defaultGLTFShader = new Shader("src/Shader/ShaderFile/gltf.vert", "src/Shader/ShaderFile/gltf.frag");
    // This will be the main Shader
    defaultShader = new Shader("src/Shader/ShaderFile/gltf.vert", "src/Shader/ShaderFile/gltf.frag");

    //defaultShader = new Shader("src/Shader/ShaderFile/default.vert", "src/Shader/ShaderFile/default.frag");
    // This is the shader just for the light bulb sprite
    LightShader = new Shader("src/Shader/ShaderFile/lightSprite.vert", "src/Shader/ShaderFile/lightSprite.frag");

    TerrainShader = new Shader("src/Shader/ShaderFile/TerrainFloor.vert", "src/Shader/ShaderFile/TerrainFloor.frag");

    TestShader = new Shader("src/Shader/ShaderFile/test.vert", "src/Shader/ShaderFile/test.frag");

    
}


