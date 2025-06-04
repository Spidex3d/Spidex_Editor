#pragma once
#include <glm\glm.hpp>
#include "../Shader/Shader.h"
#include "../Ecs/BaseModel.h"
#include "../Headers/GlobalVars.h"
#include "../Headers/Config.h"

class MainTerrain : public BaseModel {

public:
    GLuint t_VAO, t_VBO, t_EBO;
    int width, height, nrChannels;
    std::vector<float> t_vertice;
    std::vector<unsigned> t_indice;
    
    
    int rez = 1;
    int numStrips = 0;
    int vertsPerStrip = 0;
                
    MainTerrain(int idx, const std::string& name, int TerrainIdx) {
        index = idx;
        objectName = name;
        objectIndex = TerrainIdx;
        objectTypeID = TERRAIN_FLOOR;

        unsigned char* t_data = stbi_load("Textures/Terrain/Data/test_all_black_s.png", &width, &height, &nrChannels, 0);
        if (!t_data) {
            std::cout << "Failed Loading the Heightmap " << std::endl;
            return;
            
        }
        std::cout << "Loading the Heightmap of size " << height << " X " << width << std::endl;
                   
        std::vector<float> heightMap(width * height);
        float y_Scale = 64.0f / 256.0f;
        float y_Shift = 16.0f;
        unsigned bytePerPixel = nrChannels;

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                unsigned char* pixelOffset = t_data + (j + width * i) * bytePerPixel;
                unsigned char y = pixelOffset[0];
                heightMap[i * width + j] = static_cast<float>(y) * y_Scale - y_Shift;
            }
        }
        stbi_image_free(t_data);

        // Generate vertices with normals
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                float x = -height / 2.0f + height * i / static_cast<float>(height);
                float y = heightMap[i * width + j];
                float z = -width / 2.0f + width * j / static_cast<float>(width);

                // Calculate normal using surrounding heights
                float hL = (j > 0) ? heightMap[i * width + (j - 1)] : y;
                float hR = (j < width - 1) ? heightMap[i * width + (j + 1)] : y;
                float hD = (i > 0) ? heightMap[(i - 1) * width + j] : y;
                float hU = (i < height - 1) ? heightMap[(i + 1) * width + j] : y;

                glm::vec3 normal = glm::normalize(glm::vec3(hL - hR, 2.0f, hD - hU)); // y is up

                float u = j / float(width - 1);
                float v = i / float(height - 1);

                // Push position
                t_vertice.push_back(x);
                t_vertice.push_back(y);
                t_vertice.push_back(z);

                // Push normal
                t_vertice.push_back(normal.x);
                t_vertice.push_back(normal.y);
                t_vertice.push_back(normal.z);
                // Text coords
                t_vertice.push_back(u); // tex coord u
                t_vertice.push_back(v); // tex coord v
            }
        }

           /*float y_Scale = 64.0f / 256.0f, y_Shift = 16.0f;
           unsigned bytePerPixel = nrChannels;
           for (int i = 0; i < height; i++)
           {
               for (int j = 0; j < width; j++)
               {
                   unsigned char* pixelOffset = t_data + (j + width * i) * bytePerPixel;
                   unsigned char y = pixelOffset[0];
                   t_vertice.push_back(-height / 2.0f + height * i / (float)height);
                   t_vertice.push_back((int)y * y_Scale - y_Shift);
                   t_vertice.push_back(-width / 2.0f + width * j / (float)width);
               }
           }*/
           std::cout << "Loaded " << t_vertice.size() / 3 << " Vertices" << std::endl;
               
        for (unsigned i = 0; i < height - 1; i += rez)
        {
            for (unsigned j = 0; j < width; j += rez)
            {
                for (unsigned k = 0; k < 2; k++) {

                    t_indice.push_back(j + width * (i + k * rez));
                }
            }
        }
         
        std::cout << "Loaded " << t_indice.size() << " Indices" << std::endl;

         numStrips = (height - 1) / rez;
         vertsPerStrip = (width / rez) * 2 - 2; // Number of indices per triangle strip

         std::cout << "Created lattice of " << numStrips << " Strips with " << vertsPerStrip << " Triangles in each" << std::endl;
         std::cout << "Created " << numStrips * vertsPerStrip << " Triangles total" << std::endl;

        glGenVertexArrays(1, &t_VAO);
        glBindVertexArray(t_VAO);

        glGenBuffers(1, &t_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, t_VBO);
        glBufferData(GL_ARRAY_BUFFER, t_vertice.size() * sizeof(float), &t_vertice[0], GL_STATIC_DRAW);
       
        glGenBuffers(1, &t_EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, t_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, t_indice.size() * sizeof(unsigned), &t_indice[0], GL_STATIC_DRAW);
        // Vertex positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
         //Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        // Texture coordinates
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    ~MainTerrain() {
        glDeleteVertexArrays(1, &t_VAO);
        glDeleteBuffers(1, &t_VBO);
    }

    void DrawMainTerrain() {
        glBindVertexArray(t_VAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // using indices
        
        //vertsPerStrip = (width / rez) * 2;

        for (unsigned strip = 0; strip < numStrips; strip++) {
            glDrawElements(
                GL_TRIANGLE_STRIP,
                vertsPerStrip + 2,
                GL_UNSIGNED_INT,
                (void*)(sizeof(unsigned) * (vertsPerStrip + 2) * strip)); // Correct byte offset per strip  add + 2 to vertsPerStrip
                    
            /*glBindVertexArray(0);
            glUseProgram(0);*/
        }
    }
    
private:
    
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
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); //was 6
        glEnableVertexAttribArray(2);
        // Texture coordinates
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));  //was 3
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

class WaterTerrain : public BaseModel
{
    WaterTerrain(std::vector<std::unique_ptr<BaseModel>>& selectedData)
    {
        

    }

};

// OK this is the terrain Editor ImGui window which will allow us to edit the terrain, water & floor / ceiling
// Change size / update the map / change the texture and so on 

class TerrainEditor : public BaseModel
{
    TerrainEditor(std::vector<std::unique_ptr<BaseModel>>& selectedData)
    {
        ImGui::GetStyle().WindowRounding = 12.0f;

    }

};