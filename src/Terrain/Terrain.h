#pragma once

#include "stb\stb_image_write.h"
#include <glm\glm.hpp>
#include "../Shader/Shader.h"
#include "../Ecs/BaseModel.h"
#include "../Headers/GlobalVars.h"
#include "../Headers/Config.h"
#include <filesystem>

namespace fs = std::filesystem;

struct TerrainImage {
    GLuint textureID;
    std::string path;
};

// Forward declaration
unsigned int loadSaveTerrainTexture(const std::string& terrainPath);
//void RebuildTerrainFromHeightmap(const unsigned char* heightData, int width, int height);

class MainTerrain : public BaseModel {

public:
    GLuint t_VAO, t_VBO, t_EBO;
    //int width, height, nrChannels;
    
    int heightmapWidth = 0;
    int heightmapHeight = 0;
    int heightmapChannels = 0;
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

        //unsigned char* t_data = stbi_load("Textures/Terrain/Data/test_all_black_s.png",
        unsigned char* t_data = stbi_load("Textures/Terrain/Data/test_all_black_T.png",
            &heightmapWidth, &heightmapHeight, &heightmapChannels, 0);
        if (!t_data) {
            std::cout << "Failed Loading the Heightmap " << std::endl;
            return;
            
        }
        std::cout << "Loading the Heightmap of size " << heightmapHeight << " X " << heightmapWidth << std::endl;
                   
        std::vector<float> heightMap(heightmapWidth * heightmapHeight);
        float y_Scale = 64.0f / 256.0f;
        float y_Shift = 16.0f;
        unsigned bytePerPixel = heightmapChannels;

        for (int i = 0; i < heightmapHeight; ++i) {
            for (int j = 0; j < heightmapWidth; ++j) {
                unsigned char* pixelOffset = t_data + (j + heightmapWidth * i) * bytePerPixel;
               
                unsigned char y = pixelOffset[0];
                heightMap[i * heightmapWidth + j] = static_cast<float>(y) * y_Scale - y_Shift;
            }
        }
        stbi_image_free(t_data);

        // Generate vertices with normals
        for (int i = 0; i < heightmapHeight; ++i) {
            for (int j = 0; j < heightmapWidth; ++j) {
                float x = -heightmapHeight / 2.0f + heightmapHeight * i / static_cast<float>(heightmapHeight);
                float y = heightMap[i * heightmapWidth + j];
                float z = -heightmapWidth / 2.0f + heightmapWidth * j / static_cast<float>(heightmapWidth);

                // Calculate normal using surrounding heights
                float hL = (j > 0) ? heightMap[i * heightmapWidth + (j - 1)] : y;
                float hR = (j < heightmapWidth - 1) ? heightMap[i * heightmapWidth + (j + 1)] : y;
                float hD = (i > 0) ? heightMap[(i - 1) * heightmapWidth + j] : y;
                float hU = (i < heightmapHeight - 1) ? heightMap[(i + 1) * heightmapWidth + j] : y;

                glm::vec3 normal = glm::normalize(glm::vec3(hL - hR, 2.0f, hD - hU)); // y is up

                float u = j / float(heightmapWidth - 1);
                float v = i / float(heightmapHeight - 1);

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
               
           std::cout << "Loaded " << t_vertice.size() / 3 << " Vertices" << std::endl;
               
        for (unsigned i = 0; i < heightmapHeight - 1; i += rez) {
            for (unsigned j = 0; j < heightmapWidth; j += rez) {
                for (unsigned k = 0; k < 2; k++) {
                    t_indice.push_back(j + heightmapWidth * (i + k * rez));
                }
            }
        }


        std::cout << "Loaded " << t_indice.size() << " Indices" << std::endl;

         numStrips = (heightmapHeight - 1) / rez;
         vertsPerStrip = (heightmapWidth / rez) * 2 - 2; // Number of indices per triangle strip

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
    // #################################### new bit ##################################
    void ReloadTerrainMeshFromFile(const std::string& heightmapPath) {
        // Clear existing vertex/index buffers
        t_vertice.clear();
        t_indice.clear();

        int channels;
        unsigned char* newData = stbi_load(heightmapPath.c_str(), &heightmapWidth, &heightmapHeight, &channels, 1);
        if (!newData) {
            std::cerr << "Failed to reload heightmap: " << heightmapPath << "\n";
            return;
        }

        // Rebuild vertex data (same logic you used in your constructor)
        float y_Scale = 64.0f / 256.0f;
        float y_Shift = 16.0f;
        std::vector<float> heightMap(heightmapWidth * heightmapHeight);

        for (int i = 0; i < heightmapHeight; ++i) {
            for (int j = 0; j < heightmapWidth; ++j) {
                unsigned char y = newData[i * heightmapWidth + j];
                heightMap[i * heightmapWidth + j] = static_cast<float>(y) * y_Scale - y_Shift;
            }
        }

        stbi_image_free(newData);

        for (int i = 0; i < heightmapHeight; ++i) {
            for (int j = 0; j < heightmapWidth; ++j) {
                float x = -heightmapHeight / 2.0f + heightmapHeight * i / static_cast<float>(heightmapHeight);
                float y = heightMap[i * heightmapWidth + j];
                float z = -heightmapWidth / 2.0f + heightmapWidth * j / static_cast<float>(heightmapWidth);

                float hL = (j > 0) ? heightMap[i * heightmapWidth + (j - 1)] : y;
                float hR = (j < heightmapWidth - 1) ? heightMap[i * heightmapWidth + (j + 1)] : y;
                float hD = (i > 0) ? heightMap[(i - 1) * heightmapWidth + j] : y;
                float hU = (i < heightmapHeight - 1) ? heightMap[(i + 1) * heightmapWidth + j] : y;

                glm::vec3 normal = glm::normalize(glm::vec3(hL - hR, 2.0f, hD - hU));

                float u = j / float(heightmapWidth - 1);
                float v = i / float(heightmapHeight - 1);

                t_vertice.push_back(x);
                t_vertice.push_back(y);
                t_vertice.push_back(z);
                t_vertice.push_back(normal.x);
                t_vertice.push_back(normal.y);
                t_vertice.push_back(normal.z);
                t_vertice.push_back(u);
                t_vertice.push_back(v);
            }
        }

        for (unsigned i = 0; i < heightmapHeight - 1; i += rez) {
            for (unsigned j = 0; j < heightmapWidth; j += rez) {
                for (unsigned k = 0; k < 2; ++k) {
                    t_indice.push_back(j + heightmapWidth * (i + k * rez));
                }
            }
        }

        // Re-upload to GPU
        glBindBuffer(GL_ARRAY_BUFFER, t_VBO);
        glBufferData(GL_ARRAY_BUFFER, t_vertice.size() * sizeof(float), t_vertice.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, t_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, t_indice.size() * sizeof(unsigned), t_indice.data(), GL_STATIC_DRAW);
    }
    // #################################### end new bit ##################################
    ~MainTerrain() {
        glDeleteVertexArrays(1, &t_VAO);
        glDeleteBuffers(1, &t_VBO);
    }

    void DrawMainTerrain() {
        glBindVertexArray(t_VAO);
      
        for (unsigned strip = 0; strip < numStrips; strip++) {
            glDrawElements(
                GL_TRIANGLE_STRIP,
                vertsPerStrip + 2,
                GL_UNSIGNED_INT,
                (void*)(sizeof(unsigned) * (vertsPerStrip + 2) * strip)); // Correct byte offset per strip  add + 2 to vertsPerStrip
                             
        }
            glBindVertexArray(0);
    }   
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


// load all terrain textures from a folder
inline std::vector<TerrainImage> LoadTerrainImagesFromFolder(const std::string& folderPath) {
    std::vector<TerrainImage> textures;
    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            if (ext == ".png" || ext == ".jpg" || ext == ".bmp") {
                GLuint id = loadSaveTerrainTexture(entry.path().string());
                textures.push_back({ id, entry.path().string() });
            }
        }
    }
    return textures;
}

class WaterTerrain : public BaseModel
{
    WaterTerrain(std::vector<std::unique_ptr<BaseModel>>& selectedData)
    { 

    }

};

class TerrainEditor : public BaseModel
{
public:
    std::vector<TerrainImage> terrainTextures;
    bool texturesLoaded = false;
    unsigned char* heightmapData = nullptr;
    unsigned int heightmapTex = 0;
    int selectedBrush = 0;
    float brushSize = 10.0f;
    float brushStrength = 0.5f;

    int imgSize = 256;


    bool showBrushCursor = true;

    void Render(std::vector<std::unique_ptr<BaseModel>>& selectedData) {
        
            if (!showTerrainEditor) return;

            ImGui::GetStyle().WindowRounding = 12.0f;
            ImGui::Begin("Terrain Object Editor", &showTerrainEditor);

            ImGui::InputText("Terrain Name", nameBuffer, IM_ARRAYSIZE(nameBuffer));
            ImGui::TextColored(COLOR_LIGHTBLUE, ICON_FA_EDIT " Terrain Mesh Editor");
            ImGui::SeparatorText("Terrain Editor");

            ImGui::BeginTable("Edit Table", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_Reorderable);
            ImGui::TableNextColumn();
            ImGui::DragFloat3("##pos", terrain_Pos, 0.0f);
            ImGui::SameLine();
            if (ImGui::Button("Reset Position")) {
                terrain_Pos[0] = terrain_Pos[1] = terrain_Pos[2] = 0.0f;
            }

            ImGui::TableNextRow(); ImGui::TableNextColumn();
            ImGui::DragFloat3("##Scale", terrain_Scale, 0.0f);
            ImGui::SameLine();
            if (ImGui::Button("Reset Scale")) {
                terrain_Scale[0] = terrain_Scale[1] = terrain_Scale[2] = 1.0f;
            }
            ImGui::EndTable();

            // Load heightmap data (editable)
            static int heightmapW = 0, heightmapH = 0;
            if (!heightmapData) {
                int channels;
               // heightmapData = stbi_load("Textures/Terrain/Data/test_all_black_s.png", &heightmapW, &heightmapH, &channels, 1);
                heightmapData = stbi_load("Textures/Terrain/Data/test_all_black_T.png", &heightmapW, &heightmapH, &channels, 1);
                if (!heightmapData) {
                    std::cout << "Failed to load heightmap data for editing!\n";
                }
            }

            // Load OpenGL texture (for display)
            if (heightmapTex == 0) {
                //heightmapTex = loadSaveTerrainTexture("Textures/Terrain/Data/test_all_black_s.png");
                heightmapTex = loadSaveTerrainTexture("Textures/Terrain/Data/test_all_black_T.png");
            }

            if (heightmapTex != 0) {
                ImGui::BeginTable("HeightmapTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
                ImGui::TableNextColumn();
                ImGui::TextColored(COLOR_LIGHTBLUE, ICON_FA_EDIT" Current Heightmap:");

                ImVec2 imageSize(imgSize, imgSize);
                ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos(); // Top-left of image
                ImGui::Image((void*)(intptr_t)heightmapTex, imageSize);

                // Convert mouse to local coordinates
                ImVec2 mousePos = ImGui::GetMousePos();
                ImVec2 localMousePos = ImVec2(mousePos.x - cursorScreenPos.x, mousePos.y - cursorScreenPos.y);

                // If hovered and inside bounds
                if (ImGui::IsItemHovered() &&
                    localMousePos.x >= 0 && localMousePos.x < imgSize &&
                    localMousePos.y >= 0 && localMousePos.y < imgSize) {

                    ImDrawList* drawList = ImGui::GetWindowDrawList();
                    ImVec2 circleCenter = ImVec2(cursorScreenPos.x + localMousePos.x, cursorScreenPos.y + localMousePos.y);
                    drawList->AddCircle(circleCenter, brushSize, IM_COL32(255, 255, 0, 200), 32, 2.0f);

                    // Scale to real heightmap resolution
                    int centerX = static_cast<int>((localMousePos.x / imgSize) * heightmapW);
                    int centerY = static_cast<int>((localMousePos.y / imgSize) * heightmapH);

                    if (ImGui::IsMouseDown(ImGuiMouseButton_Right) || ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                        bool raise = ImGui::IsMouseDown(ImGuiMouseButton_Right);
                        int radius = static_cast<int>(brushSize);

                        for (int y = -radius; y <= radius; ++y) {
                            for (int x = -radius; x <= radius; ++x) {
                                int px = centerX + x;
                                int py = centerY + y;

                                if (px < 0 || px >= heightmapW || py < 0 || py >= heightmapH) continue;

                                float dist = std::sqrt(x * x + y * y);
                                if (dist > radius) continue;

                                //float factor = 1.0f - (dist / radius);
                                float f = 1.0f - (dist / radius);
                                float factor = f * f * (3.0f - 2.0f * f); // Smoothstep falloff

                                int index = py * heightmapW + px;
                                // new bit #################
                                // "Spray" toward white (raise) or black (lower) using blend
                                unsigned char& pixel = heightmapData[index];
                                float target = raise ? 255.0f : 0.0f;

                                // Blend toward the target based on brush strength and falloff
                                pixel = static_cast<unsigned char>(
                                    std::clamp(pixel + (target - pixel) * (brushStrength * factor), 0.0f, 255.0f));
                                // new bit #################
                               /* int value = heightmapData[index];
                                int delta = static_cast<int>(brushStrength * factor);
                                value += raise ? delta : -delta;
                                value = std::clamp(value, 0, 255);
                                heightmapData[index] = static_cast<unsigned char>(value);*/
                            }
                        }

                        // Upload modified texture
                        glBindTexture(GL_TEXTURE_2D, heightmapTex);
                        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, heightmapW, heightmapH, GL_RED, GL_UNSIGNED_BYTE, heightmapData);
                        glGenerateMipmap(GL_TEXTURE_2D);
                    }
                }

                

                ImGui::TableNextColumn();
                ImGui::TextColored(COLOR_LIGHTBLUE, ICON_FA_TOOLS " Edit Tools");
                ImGui::DragFloat("Brush Size", &brushSize, 1.0f, 1.0f, 128.0f);
                ImGui::DragFloat("Brush Strength", &brushStrength, 0.1f, 0.1f, 50.0f);

                if (ImGui::Button("Save Heightmap to File")) {
                    const char* savePath = "Textures/Terrain/Data/test_all_black_T.png";

                    if (heightmapData) {
                       
                        if (stbi_write_png(savePath, heightmapW, heightmapH, 1, heightmapData, heightmapW)) {
                            std::cout << "Saved heightmap to: " << savePath << "\n";
                            ShouldUpdateTerrain = true;
                        }
                        else {
                            std::cerr << "Failed to save heightmap!\n";
                        }
                    }
                }

                ImGui::EndTable();
            }

            // Texture picker
            if (dialogType) {
                ImGui::SeparatorText(" Texture Picker");
                ImGui::TextColored(COLOR_LIGHTBLUE, ICON_FA_IMAGE " Available Terrain Textures");

                if (!texturesLoaded) {
                    terrainTextures = LoadTerrainImagesFromFolder("Textures/Terrain/");
                    texturesLoaded = true;
                }

                int columns = 4;
                int count = 0;
                ImGui::BeginChild("TextureGrid", ImVec2(0, 300), true); // scrollable

                for (const auto& tex : terrainTextures) {
                    ImGui::PushID(tex.textureID);
                    if (ImGui::ImageButton((void*)(intptr_t)tex.textureID, ImVec2(64, 64))) {
                        objectUpdateIndex = SelectedDataManager::Instance().GetSelectedData()->objectIndex;
                        if (objectUpdateIndex != -1) {
                            ObjectVector[objectUpdateIndex]->textureID = tex.textureID;
                            creatTerrainTex = tex.textureID;
                        }
                    }
                    ImGui::PopID();

                    if (++count % columns != 0) ImGui::SameLine();
                }

                ImGui::EndChild();
            }

            ImGui::SeparatorText("Update Terrain");

            if (ImGui::Button("Update")) {
                if (strlen(nameBuffer) > 0) {
                    auto* selected = SelectedDataManager::Instance().GetSelectedData();
                    if (selected) {
                        selected->objectName = std::string(nameBuffer);
                        switch (selected->objectTypeID) {
                        case 25: ShouldUpdateFloor = true; break;
                        case 26: ShouldUpdateTerrain = true; break;
                        case 27: ShouldUpdateWater = true; break;
                        }
                        objectUpdateIndex = selected->objectIndex;
                        std::cout << "Updated: " << selected->objectName << " at index " << objectUpdateIndex << std::endl;
                    }
                }

                showTerrainEditor = false;
                creatTerrainTex = 0;
                dialogType = false;
                texturesLoaded = false;
            }

            ImGui::End();

            
    }
        
};


inline unsigned int loadSaveTerrainTexture(const std::string& terrainPath) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(terrainPath.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = (nrComponents == 1) ? GL_RED :
            (nrComponents == 3) ? GL_RGB :
            (nrComponents == 4) ? GL_RGBA : GL_RGB;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
    }
    else {
        std::cout << "Texture failed to load: " << terrainPath << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}




