#pragma once
#include <glm\glm.hpp>
#include "../Shader/Shader.h"
#include "../Ecs/BaseModel.h"
#include "../Headers/GlobalVars.h"
#include "../Headers/Config.h"

class MainTerrain : public BaseModel {

public:
    int rez = 1;
    int width, height, nrChannels;
    const int numStrips = (height - 1) / rez;
    const int numTrisPerStrip = (width / rez) * 2 - 2;

    MainTerrain(int idx, const std::string& name, int TerrainIdx) {
        index = idx;
        objectName = name;
        objectIndex = TerrainIdx;
        objectTypeID = TERRAIN_FLOOR;

        // At some p[oint this will need to be set from an open Dialog box
        //unsigned char* t_data = stbi_load("Textures/Terrain/Data/test_01.png", &width, &height, &nrChannels, 0);
        unsigned char* t_data = stbi_load("Textures/Terrain/Data/test.png", &width, &height, &nrChannels, 0);
        if (t_data) {
            std::cout << "Loading Heightmap of size " << height << " X " << width << std::endl;
        }
        else {
            std::cout << "Failed Loading Heightmap " << std::endl;
            return;
        }

        std::vector<float> t_vertices;
        float y_Scale = 64.0f / 256.0f, y_Shift = 16.0f;
        unsigned bytePerPixel = nrChannels;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                unsigned char* pixelOffset = t_data + (j + width * i) * bytePerPixel;
                unsigned char y = pixelOffset[0];
                t_vertices.push_back(-height / 2.0f + height * i / (float)height);
                t_vertices.push_back((int)y * y_Scale - y_Shift);
                t_vertices.push_back(-width / 2.0f + width * j / (float)width);
            }
        }
        std::cout << "Loaded " << t_vertices.size() / 3 << " Vertices" << std::endl;
        stbi_image_free(t_data);

        std::vector<unsigned> t_indices;
        for (unsigned i = 0; i < height - 1; i += rez)
        {
            for (unsigned j = 0; j < width; j += rez)
            {
                for (unsigned k = 0; k < 2; k++) {

                    t_indices.push_back(j + width * (i + k * rez));
                }
            }
        }
        std::cout << "Loaded " << t_indices.size() / 3 << " Indices" << std::endl;
                
        std::cout << "Created lattice of " << numStrips << " strips with " << numTrisPerStrip << " triangles each" << std::endl;
        std::cout << "Created " << numStrips * numTrisPerStrip << " Triangels total" << std::endl;

        glGenVertexArrays(1, &t_VAO);
        glBindVertexArray(t_VAO);
        // Vertices
        glGenBuffers(1, &t_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, t_VBO);
        glBufferData(GL_ARRAY_BUFFER, t_vertices.size() *sizeof(float), &t_vertices[0], GL_STATIC_DRAW);
        // Indices
        glGenBuffers(1, &t_EBO); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, t_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, t_indices.size() * sizeof(unsigned), &t_indices[0], GL_STATIC_DRAW);
        // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Normals
        //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        //glEnableVertexAttribArray(2);
        //// Texture coordinates
        //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        //glEnableVertexAttribArray(1);


        
    }

~MainTerrain() {}

void DrawMainTerrain() {

       glBindVertexArray(t_VAO);
      
       for (unsigned strip = 0; strip < numStrips; strip++)
       {
      
           glDrawElements(GL_TRIANGLE_STRIP, numTrisPerStrip + 2,
               GL_UNSIGNED_INT, (void*)(sizeof(unsigned) * (numTrisPerStrip + 2) * strip));
      
       }
       glBindVertexArray(0);
   }  

private:

    unsigned int t_VAO, t_VBO, t_EBO;
    
};


class FloorModel : public BaseModel {

public:
    GLuint VAO, VBO, EBO;

    FloorModel(int idx, const std::string& name, int TerrainIdx) {
        index = idx;
        objectName = name;
        objectIndex = TerrainIdx;
        objectTypeID = TERRAIN_FLOOR;

        // floor / ceiling 
        float vertices[] = {
            //Positions           Normals         Tex Coords
             0.5f, 0.0f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
             0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
            -0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
            -0.5f, 0.0f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Normal attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        // Texture coordinates
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    ~FloorModel() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void DrawFloor() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // using indices
        glBindVertexArray(0);
        glUseProgram(0);
    }

private:

};
