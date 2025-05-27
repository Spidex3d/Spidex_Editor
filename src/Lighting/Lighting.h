#pragma once
#include <glm\glm.hpp>
#include "../Headers/Config.h"

#include "../Shader/Shader.h"
#include "../Ecs/BaseModel.h"
#include "../Headers/GlobalVars.h"
#include "../Ecs/SelectedDataManager.h"

#include "../Windows/spx_FileDialog.h"
//#include "../Headers/GlobalVars.h"


// #######################################################################################################################
// ###########################################      LIGHTING SECTION       ###############################################
// #######################################################################################################################

class LightSprite : public BaseModel {

public:
          
    GLuint VAO, VBO, EBO;
    

    LightSprite(int idx, const std::string& name, int lightidx) {
        index = idx;
        objectName = name;
        objectIndex = lightidx;
        objectTypeID = LightSelector; // this needs setting at menue click
        // Set up a light sprite 
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

inline void ApplySunLights(Shader& shader, const glm::mat4& view, const glm::mat4& projection,
    const std::vector<std::unique_ptr<BaseModel>>& ObjectVector)
{
    
    //glm::vec3 sunDirection(0.0f, -1.0f, 0.0f);

    for (const auto& model : ObjectVector) {
        if (auto* light = dynamic_cast<LightSprite*>(model.get())) {

            if (light->lightType == 0) { // 0 = Sun
                glm::vec3 sunDirection = glm::normalize(glm::vec3(0.0f) - light->position);
                shader.setVec3("SunLight.direction", sunDirection);
                shader.setVec3("SunLight.color", light->lightColor);
                shader.setFloat("SunLight.intensity", light->intensity);
                return;
            }
        }
    }
   
    /*shader.setVec3("SunLight.direction", glm::vec3(0.0f, -1.0f, 0.0f));
    shader.setVec3("SunLight.color", glm::vec3(0.0f));
    shader.setFloat("SunLight.intensity", 0.0f);*/

    //if (ShouldAddSunLight) {
    //    for (const auto& model : ObjectVector) {
    //        if (auto* light = dynamic_cast<LightSprite*>(model.get())) {
    //            sunDirection = glm::normalize(glm::vec3(0.0f) - light->position);
    //            break; // Assuming one sun
    //        }
    //    }
    //}

    //shader.setVec3("SunLight.direction", sunDirection);
    //shader.setVec3("SunLight.color", glm::vec3(SunLightCol[0], SunLightCol[1], SunLightCol[2]));
    ////shader.setFloat("SunLight.intensity", 0.3f);
    //shader.setFloat("SunLight.intensity", SunLightIntensity);   

}
inline void ApplyPointLights(Shader& shader, const glm::mat4& view, const glm::mat4& projection,
    const std::vector<std::unique_ptr<BaseModel>>& ObjectVector)
{

    int pointLightIndex = 0;

    for (const auto& model : ObjectVector) {
        if (auto* light = dynamic_cast<LightSprite*>(model.get())) {
            if (light->lightType == 1) { // Point Light

                std::string arrayName = "PointLights[" + std::to_string(pointLightIndex) + "]";
                shader.setVec3(arrayName + ".position", light->position);
                shader.setVec3(arrayName + ".color", glm::vec3(light->lightColor));
                shader.setFloat(arrayName + ".intensity", light->intensity);
                shader.setFloat(arrayName + ".radius", 10.0f); // You can make this editable per light too

                pointLightIndex++;
            }
        }
    }

    // Optional: set number of point lights in shader if needed
    shader.setInt("numPointLights", pointLightIndex);


    //glm::vec3 pointLightPosition(2.0f, 3.0f, 1.0f);

    //
    //if (ShouldAddPointLight) {
    //    for (const auto& model : ObjectVector) {
    //        if (auto* pointlight = dynamic_cast<LightSprite*>(model.get())) {
    //            pointLightPosition = pointlight->position;
    //            break; // Assuming one point light
    //        }
    //    }
    //}

    //shader.setVec3("PointLights[0].position", pointLightPosition);
    //shader.setVec3("PointLights[0].color", glm::vec3(PointLightCol[0], PointLightCol[1], PointLightCol[2]));
    ////shader.setFloat("PointLights[0].intensity", 1.0f);
    //shader.setFloat("PointLights[0].intensity", PointLightIntensity);
    //shader.setFloat("PointLights[0].radius", 10.0f);
}












