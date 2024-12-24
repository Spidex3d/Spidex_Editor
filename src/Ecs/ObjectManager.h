#pragma once
#include "BaseModel.h"
//#include <glad\glad.h>




class CubeModel : public BaseModel {
public:
    float scaleX, scaleY, scaleZ;
    float posX, posY, posZ;

    CubeModel(int idx, const std::string& name, float w, float h, float d, float x, float y, float z) {
        index = idx;             // list index
        objectName = name;       // cube name
        objectIndex = 0;         // start index for a cube
        objectTypeID = OBJ_CUBE; // Example type ID for Cube set in config
        scaleX = w;
        scaleY = h;
        scaleZ = d;
        posX = x;
        posY = y;
        posZ = z;
    }

    void RenderImGui() const override {
        ImGui::Text("Cube: %s", objectName.c_str());
        ImGui::Text("Scale: %.1f x %.1f x %.1f", scaleX, scaleY, scaleZ);
        ImGui::Text("Position: %.1f x %.1f x %.1f", posX, posY, posZ);
    }
};

class SphereModel : public BaseModel {
public:
    float radius;
        SphereModel(int idx, const std::string& name, float r) {
        index = idx;                // list index
        objectName = name;          // sphere name
        objectIndex = 0;            // start index for a sphere
        objectTypeID = OBJ_SPHERE;  // Example type ID for Sphere set in config
        radius = r;
    }

    void RenderImGui() const override {
        ImGui::Text("Sphere: %s", objectName.c_str());
        ImGui::Text("Radius: %.1f", radius);
    }
};

class TriangelModel : public BaseModel {
public:
        
    TriangelModel(int idx, const std::string& name) {
        index = idx;                  // list index
        objectName = name;            // sphere name
        objectIndex = 0;              // start index for a sphere
        objectTypeID = OBJ_TRIANGEL;  // Example type ID for Sphere set in config        
    }
    void RenderImGui() const override {
        ImGui::Text("Triangel: %s", objectName.c_str());      
    }
    void Triangel() {

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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    void DrawTriangel() {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }
    
};

