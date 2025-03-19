#pragma once
#include <glm\glm.hpp>
#include "../Shader/Shader.h"
#include "BaseModel.h"
#include "../Headers/GlobalVars.h"


class MainGrid : public BaseModel {

public:
    GLuint GridVAO, GridVBO;
    std::vector<float> gridVertices;

    MainGrid(int idx, const std::string& name, float size, int divisions) {
        index = idx;             // list index
        objectName = name;       // grid name
        objectIndex = idx;       // grid index
        objectTypeID = MAIN_GRID;        // Example type ID for Grid = 0

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

};


class CubeModel : public BaseModel {
public:
    GLuint VAO, VBO;
    
    CubeModel(int idx, const std::string& name, int Cubeobjidx) {
        index = idx;             // list index
        objectName = name;       // cube name
        objectIndex = Cubeobjidx;         // start index for a cube
        objectTypeID = OBJ_CUBE; // Example type ID for Cube set in config = 1
       


        position = glm::vec3(0.0f, 0.0f, 0.0f); // Initial position
        scale = glm::vec3(1.0f, 1.0f, 1.0f);    // Initial scale
        modelMatrix = glm::mat4(1.0f);

        
        GLfloat vertices[] = { //  Normal         Tex cords
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
        // vertices
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // vertices
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Texture location 1
        //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)3);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        // Normal attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glEnableVertexAttribArray(0);
    }

    ~CubeModel() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
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
    GLuint VAO, VBO, EBO;

    PlaneModel(int idx, const std::string& name, int Planeobjidx) {
        index = idx;
        objectName = name;
        objectIndex = Planeobjidx;
        objectTypeID = OBJ_PLANE;

        
        float vertices[] = {
            //                   Text coords
             0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
             0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, 0.0f,  0.0f, 1.0f
        };
        unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
         // Vertex positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Texture coordinates
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    ~PlaneModel() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void DrawPlane() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // using indices
        glBindVertexArray(0);
        glUseProgram(0);
    }

private:
    
};


class PyramidModel : public BaseModel {

public:
    GLuint VAO, VBO;

    PyramidModel(int idx, const std::string& name, int Pyramidobjidx) {
        index = idx;
        objectName = name;
        objectIndex = Pyramidobjidx;
        objectTypeID = OBJ_PYRAMID;

        float pyramidvertices[] =
        {
            // Bottom face
             -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
              0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
              0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
              0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
             -0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f,
             -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
             // Front face
             -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
              0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
              0.0f,  0.0f,  0.5f,    0.0f, 0.0f, 1.0f,    0.5f, 1.0f,
               // Back face
             -0.5f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
              0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
              0.0f,  0.0f,  0.5f,    0.0f, 0.0f, 1.0f,    0.5f, 1.0f,
                 // Left face
             -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
             -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
              0.0f,  0.0f,  0.5f,    0.0f, 0.0f, 1.0f,    0.5f, 1.0f,
                   // Right face
              0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
              0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
              0.0f,  0.0f,  0.5f,    0.0f, 0.0f, 1.0f,    0.5f, 1.0f,
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidvertices), pyramidvertices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // Texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    ~PyramidModel() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void DrawPyramid() {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 18);
        glBindVertexArray(0);
        glUseProgram(0);
    }
private:

};




