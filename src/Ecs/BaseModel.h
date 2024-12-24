#pragma once
#include <string>
#include "../Headers/Config.h"

class BaseModel {
public:
    int index;              // list index
    std::string objectName; // objects name
    int objectIndex;        // objects index ie: cube index of type cube
    int objectTypeID;       // cube, light, camera, sphere, plane, obj model

    virtual ~BaseModel() = default;
    virtual void RenderImGui() const = 0; // Pure virtual function for ImGui rendering

};
