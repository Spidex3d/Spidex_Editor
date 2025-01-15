#include "Headers/Render.h"


void Render::RenderGrid(const glm::mat4& view, const glm::mat4& projection)
{
   
      ShaderManager::defaultGridShader->Use();
      ShaderManager::defaultGridShader->setMat4("projection", projection);
      ShaderManager::defaultGridShader->setMat4("view", view);
      modelMatrix = glm::mat4(1.0f);
      modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
      modelMatrix = glm::scale(modelMatrix, glm::vec3(20.0f, 0.0f, 20.0f));
      ShaderManager::defaultGridShader->setMat4("model", modelMatrix);

        Grid::Instance()->gridRender(); // Render the Grid
    
}
        // render triagle
void Render::RenderObjects(const glm::mat4& view, const glm::mat4& projection,
            const std::vector<std::unique_ptr<BaseModel>>& models) {
    ShaderManager::defaultGridShader->Use();
    ShaderManager::defaultGridShader->setMat4("projection", projection);
    ShaderManager::defaultGridShader->setMat4("view", view);

    for (const auto& model : models) {
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        ShaderManager::defaultGridShader->setMat4("model", modelMatrix);

        if (auto* triangle = dynamic_cast<TriangleModel*>(model.get())) {
            triangle->DrawTriangle();
        }
    }
}
void Render::RenderPlane(const glm::mat4& view, const glm::mat4& projection,
    const std::vector<std::unique_ptr<BaseModel>>& models) {
    ShaderManager::defaultGridShader->Use();
    ShaderManager::defaultGridShader->setMat4("projection", projection);
    ShaderManager::defaultGridShader->setMat4("view", view);

    for (const auto& model : models) {
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(3.0f, 0.0f, 0.0f));
        
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        ShaderManager::defaultGridShader->setMat4("model", modelMatrix);

        if (auto* plane = dynamic_cast<PlaneModel*>(model.get())) {
            plane->DrawPlane();
        }
    }
}

void Render::RenderCube(const glm::mat4& view, const glm::mat4& projection,
    const std::vector<std::unique_ptr<BaseModel>>& models)
        {

        ShaderManager::defaultGridShader->Use();
        ShaderManager::defaultGridShader->setMat4("projection", projection);
        ShaderManager::defaultGridShader->setMat4("view", view);

        for (const auto& model : models) {
            modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.0f, 0.0f, 0.0f));

            modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
            ShaderManager::defaultGridShader->setMat4("model", modelMatrix);

            if (auto* cube = dynamic_cast<CubeModel*>(model.get())) {
                cube->DrawCube();
            }
        }
}



