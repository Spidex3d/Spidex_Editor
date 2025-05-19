#pragma once
#include <glm\glm.hpp>

#include "../Shader/Shader.h"
#include "../Ecs/BaseModel.h"
#include "../Headers/GlobalVars.h"

// #######################################################################################################################
// ###########################################      LIGHTING SECTION       ###############################################
// #######################################################################################################################


class SunLight : public BaseModel {
public:
    glm::vec3 direction = glm::normalize(glm::vec3(-0.2f, -1.0f, -0.3f));
    glm::vec3 color = glm::vec3(1.0f);
    float intensity = 1.0f;

    SunLight(int idx, const std::string& name, int lightidx) {
        index = idx;
        objectName = name;
        objectIndex = lightidx;
        objectTypeID = LIGHT_SUN;
    }

    void ApplyToShader(GLuint shaderID) {
        glUniform3fv(glGetUniformLocation(shaderID, "sun.direction"), 1, glm::value_ptr(direction));
        glUniform3fv(glGetUniformLocation(shaderID, "sun.color"), 1, glm::value_ptr(color));
        glUniform1f(glGetUniformLocation(shaderID, "sun.intensity"), intensity);
    }

};
class PointLight : public BaseModel {

};
class SpotLight : public BaseModel {

};
class AreaLight : public BaseModel {

};

class LightSprite : public BaseModel {

public:
    GLuint VAO, VBO, EBO;

    LightSprite(int idx, const std::string& name, int lightidx) {
        index = idx;
        objectName = name;
        objectIndex = lightidx;
        objectTypeID = LIGHT_SUN;

        float vertices[] = {
            //Positions          Normals          Text coords
             0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,1.0f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f

        };
        unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        // Vertex positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Normal attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        // Texture coordinates
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    ~LightSprite() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void DrawLight() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // using indices
        glBindVertexArray(0);
        glUseProgram(0);
    }

private:

};


// glm::vec3 lightPos;

//float ambientFactor(ambient_factor[0]);
//glm::vec3 lightColor(amb_light[0], amb_light[1], amb_light[2]);
//
//ShaderManager::LightCubeShader->Use();
//ShaderManager::LightCubeShader->setVec3("light.position", lightPos);
//ShaderManager::LightCubeShader->setVec3("viewPos", camera.Position);
//ShaderManager::LightCubeShader->setVec3("light.ambient", 1.8f, 1.8f, 1.8f);
//ShaderManager::LightCubeShader->setVec3("light.diffuse", 1.5f, 1.5f, 1.5f);
//ShaderManager::LightCubeShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
//
//ShaderManager::LightCubeShader->setVec3("matirial.specular", 0.5f, 0.5f, 0.5f);
//ShaderManager::LightCubeShader->setFloat("matirial.shininess", 60.0f);

//glm::vec3 viewPos;
//
//glm::vec3 lightPos(0.0f, 1.0f, 1.0f);
//
////glm::vec3 lightColor(amb_light[0], amb_light[1], amb_light[2]);
//
//angel += (float)deltaTime * 50.0f;
//lightPos.x = 4.0f * sinf(glm::radians(angel));

//ShaderManager::LightCubeShader->Use();
//ShaderManager::LightCubeShader->setMat4("view", view);
//ShaderManager::LightCubeShader->setMat4("projection", projection);
//ShaderManager::LightCubeShader->setVec3("viewPos", viewPos);
//ShaderManager::LightCubeShader->setVec3("lightColor", lightColor);
//ShaderManager::LightCubeShader->setVec3("lightPos", lightPos);
//ShaderManager::LightCubeShader->setFloat("ambientFactor", ambientFactor);



// ################################ Draw light sphere obj ###############################
//model = glm::translate(model = glm::mat4(1.0f), lightPos);
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
//ShaderManager::LightBulbShader->Use();
//
//ShaderManager::LightBulbShader->setVec3("lightColor", amb_light[0], amb_light[1], amb_light[2]);
//ShaderManager::LightBulbShader->setMat4("model", model);
//ShaderManager::LightBulbShader->setMat4("view", view);
//ShaderManager::LightBulbShader->setMat4("projection", projection);
//
//lightMesh.objDraw();
