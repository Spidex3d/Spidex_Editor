#include "DefaultModels.h"


DefaultModels* DefaultModels::Instance()
{

    static DefaultModels* defaultModel = new DefaultModels;
    return defaultModel;
}

//void DefaultModels::TriangleModel(int idx, const std::string& name)
void DefaultModels::TriangleModel()
{
    
        /*index = idx;
        objectName = name;*/
        objectIndex = 0;
        objectTypeID = OBJ_TRIANGEL;

        float vertices[] = {
            -1.0f, -1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
             0.0f,  1.0f, 0.0f
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    
}

void DefaultModels::RenderImGui() const
{
    ImGui::Text("Triangle: %s", objectName.c_str());
}

void DefaultModels::DrawTriangle()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glUseProgram(0);
}
