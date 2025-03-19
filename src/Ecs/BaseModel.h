#pragma once
#include <string>
#include <glm\glm.hpp>
#include "../Headers/Config.h"

class BaseModel {
public:
    int index;              // list index
    std::string objectName; // objects name
    int objectIndex;        // objects index ie: cube index of type cube
    int objectTypeID;       // cube, light, camera, sphere, plane, obj model

    glm::vec3 position;     // Position of the object
    glm::vec3 scale;        // Scale of the object
    glm::vec3 rotation;        // Scale of the object
    glm::mat4 modelMatrix;  // Model matrix for transformations

    bool isSelected;    // this for the selection in the scene list box

    unsigned int textureID;

    unsigned int CubeTextureID;
    unsigned int creatMap = 0;
     


    virtual ~BaseModel() = default;
    //virtual void RenderImGui() const = 0; // Pure virtual function for ImGui rendering

};
