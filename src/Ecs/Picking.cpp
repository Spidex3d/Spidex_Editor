#include "Picking.h"

Picking* Picking::Instance() {
    static Picking instance;
    return &instance;
}

void Picking::Initialize() {
   
    
}

Picking Picking::createRayFromMouse(float mouseX, float mouseY, const Camera& camera, float windowWidth, float windowHeight) {
    // Convert mouse position to normalized device coordinates (NDC)
    float ndcX = (2.0f * mouseX) / windowWidth - 1.0f;
    float ndcY = 1.0f - (2.0f * mouseY) / windowHeight; // Note: Y is inverted

    // Create a vector in clip space
    glm::vec4 clipCoords(ndcX, ndcY, -1.0f, 1.0f);

    // Transform from clip space to view space
    glm::mat4 invProj = glm::inverse(camera.GetProjectionMatrix(windowWidth / windowHeight));
    glm::vec4 viewCoords = invProj * clipCoords;
    viewCoords.z = -1.0f;
    viewCoords.w = 0.0f;

    // Transform from view space to world space
    glm::mat4 invView = glm::inverse(camera.GetViewMatrix());
    glm::vec4 worldCoords = invView * viewCoords;
    glm::vec3 rayDirection = glm::normalize(glm::vec3(worldCoords));

    return Picking(camera.Position, rayDirection);
}

void Picking::ObjectPicking(const Camera& camera, float windowWidth, float windowHeight) {
    // Get mouse position
    ImVec2 mousePos = ImGui::GetMousePos();
    ImVec2 windowPos = ImGui::GetWindowPos();

    relativeX = mousePos.x - windowPos.x; //this needs to be the window we are clicking in MainSceneWindow
    relativeY = mousePos.y - windowPos.y;

    // Create ray from mouse position
    Picking ray = createRayFromMouse(relativeX, relativeY, camera, windowWidth, windowHeight);

    std::cout << "The mouse x (relative): " << relativeX << " The mouse y (relative): " << relativeY << std::endl;
    
    // Handle object picking logic with ray
    // Your logic to check for intersections with objects goes here
}

// ########################################


    