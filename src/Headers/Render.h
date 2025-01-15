#pragma once
#include <glm/glm.hpp>
#include "Shader/ShaderManager.h"
#include "Editor_Gui/Grid.h"
#include "Ecs/EntityNodes.h"


class Render
{
public:
    void RenderGrid(const glm::mat4& view, const glm::mat4& projection);
    void RenderObjects(const glm::mat4& view, const glm::mat4& projection, const std::vector<std::unique_ptr<BaseModel>>& models);
    void RenderPlane(const glm::mat4& view, const glm::mat4& projection, const std::vector<std::unique_ptr<BaseModel>>& models);
    void RenderCube(const glm::mat4& view, const glm::mat4& projection, const std::vector<std::unique_ptr<BaseModel>>& models);

private:
    glm::mat4 modelMatrix;

};

