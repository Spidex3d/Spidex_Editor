#pragma once
#include <glm\glm.hpp>
#include "../Shader/Shader.h"
#include "../Headers/GlobalVars.h"

class BaseModel {
public:
    int index;              // list index
    std::string objectName; // objects name
    int objectIndex;        // objects index ie: cube index of type cube
    int objectTypeID;       // cube, light, camera, sphere, plane, obj model

    glm::vec3 position;

    virtual ~BaseModel() = default;
    virtual void RenderImGui() const = 0; // Pure virtual function for ImGui rendering
    
};

class MainGrid : public BaseModel {

public:
    GLuint GridVAO, GridVBO;
    std::vector<float> gridVertices;

    MainGrid(int idx, const std::string& name, float size, int divisions) {
        index = idx;             // list index
        objectName = name;       // grid name
        objectIndex = idx;       // grid index
        objectTypeID = MAIN_GRID;        // Example type ID for Grid

        gridVertices = createGridVertices(size, divisions);
        SetUp();
    }

    std::vector<float> createGridVertices(float size, int divisions) {
        std::vector<float> vertices;
        float step = size / divisions;
        float halfSize = size / 2.0f;

        for (int i = 0; i <= divisions; ++i) {
            float coord = -halfSize + i * step;
            // Vertical lines
            vertices.push_back(coord);
            vertices.push_back(0.0f);
            vertices.push_back(-halfSize);
            vertices.push_back(coord);
            vertices.push_back(0.0f);
            vertices.push_back(halfSize);
            // Horizontal lines
            vertices.push_back(-halfSize);
            vertices.push_back(0.0f);
            vertices.push_back(coord);
            vertices.push_back(halfSize);
            vertices.push_back(0.0f);
            vertices.push_back(coord);
        }
        return vertices;
    }

    void SetUp() {
        glGenVertexArrays(1, &GridVAO);
        glGenBuffers(1, &GridVBO);

        glBindVertexArray(GridVAO);
        glBindBuffer(GL_ARRAY_BUFFER, GridVBO);
        glBufferData(GL_ARRAY_BUFFER, gridVertices.size() * sizeof(float), gridVertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    //void DrawGrid() const override {
    void DrawGrid()  {
        glBindVertexArray(GridVAO);
        glDrawArrays(GL_LINES, 0, gridVertices.size() / 3);
        glBindVertexArray(0);
    }

    void RenderImGui() const override {
       // ImGui::Text("Grid: %s", objectName.c_str());
    }
};


class CubeModel : public BaseModel {
public:
    GLuint VAO, VBO;
    //float scaleX, scaleY, scaleZ;
    //float posX, posY, posZ;

   // CubeModel(int idx, const std::string& name, int Cubeobjidx, float w, float h, float d, float x, float y, float z) {
    CubeModel(int idx, const std::string& name, int Cubeobjidx) {
        index = idx;             // list index
        objectName = name;       // cube name
        objectIndex = Cubeobjidx;         // start index for a cube
        objectTypeID = OBJ_CUBE; // Example type ID for Cube set in config
       /* scaleX = w;
        scaleY = h;
        scaleZ = d;
        posX = x;
        posY = y;
        posZ = z;*/

        GLfloat vertices[] = { //     Normal          Tex cords
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,0.0f, 1.0f

        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // Texture location 1
        //glEnableVertexAttribArray(1);
        //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)3);
        glEnableVertexAttribArray(1);
        //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        // Normal attribute
        //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glEnableVertexAttribArray(0);
    }

    ~CubeModel() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void RenderImGui() const override {
        ImGui::Text("Cube: %s", objectName.c_str());
        //ImGui::Text("Scale: %.1f x %.1f x %.1f", scaleX, scaleY, scaleZ);
       // ImGui::Text("Position: %.1f x %.1f x %.1f", posX, posY, posZ);
    }

    void DrawCube()  {

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }
    private:
};

class SphereModel : public BaseModel {
public:
    float radius;
        SphereModel(int idx, const std::string& name, int Sphereobjidx, float r) {
        index = idx;                // list index
        objectName = name;          // sphere name
        objectIndex = Sphereobjidx;            // start index for a sphere
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

    TriangleModel(int idx, const std::string& name, int Triangleobjidx) {
        index = idx;
        objectName = name;
        objectIndex = Triangleobjidx;
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
  
};

class PlaneModel : public BaseModel {

public:
    GLuint VAO, VBO;

    PlaneModel(int idx, const std::string& name, int Planeobjidx) {
        index = idx;
        objectName = name;
        //objectIndex = 0;
        objectIndex = Planeobjidx;
        objectTypeID = OBJ_PLANE;

        float vertices[] = {
           
            -0.5f, -0.5f, -0.5f, // top triangel
             0.5f, -0.5f, -0.5f, // top triangel
             0.5f,  0.5f, -0.5f, // top triangel   front
             0.5f,  0.5f, -0.5f, // bottom triangel
            -0.5f,  0.5f, -0.5f, // bottom triangel
            -0.5f, -0.5f, -0.5f  // bottom triangel

           /*-0.5f,  0.5f,  0.5f,
           -0.5f,  0.5f, -0.5f,
           -0.5f, -0.5f, -0.5f, // left right
           -0.5f, -0.5f, -0.5f,
           -0.5f, -0.5f,  0.5f,
           -0.5f,  0.5f,  0.5f*/
           
          /*  -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f, // top
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,*/

            //- 0.5f, -0.5f, -0.5f,
            // 0.5f, -0.5f, -0.5f,
            // 0.5f, -0.5f,  0.5f, // bottom
            // 0.5f, -0.5f,  0.5f,
            //-0.5f, -0.5f,  0.5f,
            //-0.5f, -0.5f, -0.5f
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

    ~PlaneModel() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void RenderImGui() const override {
        ImGui::Text("Triangle: %s", objectName.c_str());
    }

    // void DrawTriangle(GLuint shaderProgramID) {
    void DrawPlane() {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glUseProgram(0);
    }
private:
    
};



