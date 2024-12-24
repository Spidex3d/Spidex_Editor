#pragma once

#include <vector>
#include <memory>
//#include "Object.h"// Make sure to include Object.h which defines BaseModel and derived classes
#include "ObjectManager.h" // this replaced Object.h
class EntityComponents {
public:
    EntityComponents();
    ~EntityComponents();

    static EntityComponents* Instance();
    void Initialize();
    std::vector<std::unique_ptr<BaseModel>>& GetModels();

private:
    std::vector<std::unique_ptr<BaseModel>> MainVector;
    int currentIndex;
    int indexCube;
    int indexPlane;
    int indexSphere;
    int indexLight;
    int indexTypeID;
};
