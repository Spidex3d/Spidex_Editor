#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Camera/Camera.h"
#include "../Headers/GlobalVars.h"


// #include "../Windows/MainScreen.h"

#include <iostream>
//#include <vector>

class Picking
{
public:
    glm::vec3 origin;
    glm::vec3 direction;

    Picking() : origin(0.0f), direction(0.0f) {}
    Picking(const glm::vec3& orig, const glm::vec3& dir) : origin(orig), direction(dir) {}

    static Picking* Instance();
    void Initialize();
    void ObjectPicking(const Camera& camera, float windowWidth, float windowHeight);

private:
    float relativeX;
    float relativeY;

    Picking createRayFromMouse(float mouseX, float mouseY, const Camera& camera, float windowWidth, float windowHeight);
};