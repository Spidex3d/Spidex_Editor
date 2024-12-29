#include "EntityComponents.h"



BaseModel* selectedData = nullptr;  // Define the external variable

EntityComponents::EntityComponents()
    : currentIndex(0), indexCube(0), indexPlane(0), indexSphere(0), indexLight(0), indexTypeID(0) {
                 // -1
}

EntityComponents::~EntityComponents() {
}

EntityComponents* EntityComponents::Instance() {
    static EntityComponents component; // Singleton implementation without using new
    return &component;
}

void EntityComponents::Initialize() {
    
    MainVector.push_back(std::make_unique<CubeModel>(currentIndex++, "DefaultCube", 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 4.0f));
    MainVector.push_back(std::make_unique<SphereModel>(currentIndex++, "DefaultSphere", 1.0f));
    MainVector.push_back(std::make_unique<SphereModel>(currentIndex++, "DefaultSphere", 1.0f));
    MainVector.push_back(std::make_unique<SphereModel>(currentIndex++, "DefaultSphere", 1.0f));
    MainVector.push_back(std::make_unique<TriangleModel>(currentIndex++, "DefaultTriangel"));
}

 std::vector<std::unique_ptr<BaseModel>>& EntityComponents::GetModels() {
    return MainVector;
}


