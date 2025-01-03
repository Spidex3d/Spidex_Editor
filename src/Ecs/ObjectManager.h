#pragma once
#include "BaseModel.h"
#include "../Shader/Shader.h"

struct Object {

    int index = 0;              // list index
    std::string objectName; // objects name
    int objectIndex;        // objects index ie: cube index of type cube
    int objectTypeID;       // cube, light, camera, sphere, plane, obj model
};


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

class TriangleModel : public BaseModel {

public:
    GLuint VAO, VBO;

    TriangleModel(int idx, const std::string& name) {
        index = idx;
        objectName = name;
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

    ~TriangleModel() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void RenderImGui() const override {
        ImGui::Text("Triangle: %s", objectName.c_str());
    }

   // void DrawTriangle(GLuint shaderProgramID) {
    void DrawTriangle() {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glUseProgram(0);
    }
private:
    //int index;              // list index
    //std::string objectName; // objects name
    //int objectIndex;        // objects index ie: cube index of type cube
    //int objectTypeID;       // cube, light, camera, sphere, plane, obj model
};



