#pragma once
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include "Shader.h"

class ShaderManager {
public:
    static void SetupShaders();
    static Shader* defaultGridShader; // Declare the shader as a static member
    static Shader* defaultGLTFShader; // Declare the shader as a static member
    static Shader* LightBulbShader; // Declare the shader as a static member
    static Shader* LightShader; // Declare the shader as a static member
    static Shader* defaultShader; // Declare the shader as a static member
    static Shader* skyShader; // Declare the shader as a static member
    static Shader* TerrainShader; // Declare the shader as a static member
    static Shader* TestShader; // Declare the shader as a static member
    static Shader* highlightShader;
    static Shader* LineShader;
};
