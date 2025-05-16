#pragma once
#include "../Ecs/BaseModel.h"
#include "../Headers/GlobalVars.h"
#include "../Shader/ShaderManager.h"

#include "../Shader/Shader.h"
#include "../Camera/Camera.h"

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

#include <stb\stb_image.h>

#include <json/json.hpp>


using json = nlohmann::json;

struct SubMesh {
    GLuint gltfVAO = 0;
    GLuint gltfVBO = 0;
    GLuint gltfEBO = 0;
    GLuint textureID = 0;
    size_t indexCount = 0;
    std::map<std::string, GLuint> textures;  // e.g., "baseColor", "normal", etc.
};

struct GLTFMesh {
    std::vector<SubMesh> submeshes;
};
// this is a forward declaration  
GLTFMesh LoadGLTFMesh(const std::string& gltfPath, const std::string& binPath);
inline std::map<std::string, GLuint> loadTextureForMaterial(const json& gltf, int materialIndex, const std::string& gltfPath);
//GLuint loadTextureForMaterial(const json& gltf, int materialIndex, const std::string& gltfPath);

class Gltf : public BaseModel {

public:
    GLTFMesh mesh;

    Gltf(int idx, const std::string& name, int glTFModelIndex)
        : idx(idx), name(name), glTFModelIndex(glTFModelIndex) {
     
        // This is used fo the scene list box and selection for editing the object
        index = idx;                    // list index
        objectName = name;              // gltf file name
        objectIndex = glTFModelIndex;   // start index for a gltf
        objectTypeID = GLTF_MODEL;      // Example type ID for gltf set in config = 14
    }

    bool Load(const std::string& gltfPath, const std::string& binPath) {
        try {
            mesh = LoadGLTFMesh(gltfPath, binPath); // From your gltf.h
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "GLTF Load Error: " << e.what() << std::endl;
            return false;
        }
    }

    // Draw Function
    void Draw(Shader& shader, Camera& camera) {
        std::vector<std::string> textureOrder = {
             "baseColor", "normalMap", "metallicRoughnessMap", "occlusionMap", "emissiveMap"
        };
              
        for (const auto& sub : mesh.submeshes) {
            int texUnit = 0; 

            for (const auto& texName : textureOrder) {

                auto it = sub.textures.find(texName);
                if (it != sub.textures.end()) {
                    GLuint textureID = it->second;

                    glActiveTexture(GL_TEXTURE0 + texUnit); // for this file this should be one
                    glBindTexture(GL_TEXTURE_2D, textureID);
                    //shader.setInt(texName, texUnit);
                    ShaderManager::defaultShader->Use();
                    ShaderManager::defaultShader->setInt(texName, texUnit);

                    std::cout << "Binding texture: " << texName << " to unit " << texUnit

                        << " with ID: " << textureID << std::endl;

                    texUnit++;
                }
                else {
                    std::cout << "Texture not found for: " << texName << std::endl;


                  
                }
                    ShaderManager::defaultShader->setMat4("model", modelMatrix);
                               
                 if (sub.gltfVAO != 0) {
                     glBindVertexArray(sub.gltfVAO);
                     glDrawElements(GL_TRIANGLES, sub.indexCount, GL_UNSIGNED_SHORT, 0);
                     glBindVertexArray(0);
                 }
                                  
                 glActiveTexture(GL_TEXTURE0);
            }
        }
    }
    
private:
    int idx;
    const std::string& name;
    int glTFModelIndex;

};
// read the text part of the file
inline std::string ReadTextFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) throw std::runtime_error("Failed to read file: " + path);
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

// read the binary part of the file
inline std::vector<unsigned char> ReadBinaryFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) throw std::runtime_error("Failed to read binary: " + path);
    std::vector<unsigned char> buffer(file.tellg());
    file.seekg(0);
    file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
    return buffer;
}

template <typename T>
inline T* GetAccessorData(const json& gltf, const std::vector<unsigned char>& buffer, const json& accessor) {
    const auto& bufferView = gltf["bufferViews"][accessor["bufferView"].get<int>()];
    size_t offset = bufferView.value("byteOffset", 0) + accessor.value("byteOffset", 0);
    return reinterpret_cast<T*>(const_cast<unsigned char*>(buffer.data() + offset));
}

inline GLTFMesh LoadGLTFMesh(const std::string& gltfPath, const std::string& binPath) {
    json gltf = json::parse(ReadTextFile(gltfPath));
    std::vector<unsigned char> binData = ReadBinaryFile(binPath);

    GLTFMesh result;
   
    // ###################                 #################### 
    // ################### Working on this ####################
    if (gltf.contains("name")) {
        const std::string& gltfname = gltf["name"];
        std::cout << "Mesh Name: " << gltfname << std::endl;
    }
    else {
        std::cerr << "Mesh does not have a name." << std::endl;
    }
    /*const auto& primitives = gltf["name"][0]["name"];
    const std::string& gltfname = gltf["name"];
    std::cout << "Mesh Name: " << gltfname << std::endl;*/
        
    // ##########################################################


    const auto& primitives = gltf["meshes"][0]["primitives"];
    // position
    for (const auto& primitive : primitives) {
        const auto& posAcc = gltf["accessors"][primitive["attributes"]["POSITION"].get<int>()];
        float* positions = GetAccessorData<float>(gltf, binData, posAcc);
        // normals
        const auto& normAcc = primitive["attributes"].contains("NORMAL")
            ? gltf["accessors"][primitive["attributes"]["NORMAL"].get<int>()]
            : json{};
        float* normals = normAcc.is_null() ? nullptr : GetAccessorData<float>(gltf, binData, normAcc);
        // uvs tex coords
        const auto& uvAcc = primitive["attributes"].contains("TEXCOORD_0")
            ? gltf["accessors"][primitive["attributes"]["TEXCOORD_0"].get<int>()]
            : json{};
        float* uvs = uvAcc.is_null() ? nullptr : GetAccessorData<float>(gltf, binData, uvAcc);

        size_t vertexCount = posAcc["count"];
        std::vector<float> vertexBuffer;
        for (size_t i = 0; i < vertexCount; ++i) {
                // position
                vertexBuffer.push_back(positions[i * 3 + 0]);
                vertexBuffer.push_back(positions[i * 3 + 1]);
                vertexBuffer.push_back(positions[i * 3 + 2]);

            // normal
            if (normals) {
                vertexBuffer.push_back(normals[i * 3 + 0]);
                vertexBuffer.push_back(normals[i * 3 + 1]);
                vertexBuffer.push_back(normals[i * 3 + 2]);
            }
            else {
                vertexBuffer.push_back(0.0f);
                vertexBuffer.push_back(0.0f);
                vertexBuffer.push_back(0.0f);
            }
            // tex coords
            if (uvs) {
                float u = uvs[i * 2 + 0];
                float v = 1.0f - uvs[i * 2 + 1]; // <-- FLIP V coordinate
                vertexBuffer.push_back(u); // [i * 2 + 0] );
                vertexBuffer.push_back(v); // [i * 2 + 1] );
            }
            else {
                vertexBuffer.push_back(0.0f);
                vertexBuffer.push_back(0.0f);
            }
        }

       
        std::vector<uint16_t> indices;
        if (primitive.contains("indices")) {
            const auto& idxAcc = gltf["accessors"][primitive["indices"].get<int>()];
            uint16_t* idxData = GetAccessorData<uint16_t>(gltf, binData, idxAcc);
            indices.assign(idxData, idxData + idxAcc["count"].get<size_t>());
        }

        SubMesh sub;
        glGenVertexArrays(1, &sub.gltfVAO);
        glGenBuffers(1, &sub.gltfVBO);
        glGenBuffers(1, &sub.gltfEBO);

        glBindVertexArray(sub.gltfVAO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sub.gltfEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint16_t), indices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, sub.gltfVBO);
        glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(float), vertexBuffer.data(), GL_STATIC_DRAW);

        float stride = vertexBuffer.size() / vertexCount;
        // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        // texture pos
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        // Tangent
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(3);

        sub.indexCount = indices.size();
        sub.textureID = 0;
               
        std::cout << "Total floats: " << vertexBuffer.size() << std::endl;
        std::cout << "Total vertices: " << vertexCount << std::endl;
        std::cout << "Floats per vertex: " << (float)vertexBuffer.size() / vertexCount << std::endl;

        if (primitive.contains("material")) {
            int materialIndex = primitive["material"];
            //sub.textureID = loadTextureForMaterial(gltf, materialIndex, gltfPath);
            sub.textures = loadTextureForMaterial(gltf, materialIndex, gltfPath);
        }

        glBindVertexArray(0);

        result.submeshes.push_back(sub);
    }

    return result;
    
}
// NEW ###########################
inline GLuint loadTextureFromImageIndex(const json& gltf, int imageIndex, const std::string& gltfPath) {
    if (imageIndex >= gltf["images"].size()) return 0;

    std::string imagePath = gltf["images"][imageIndex]["uri"];
    std::string baseDir = gltfPath.substr(0, gltfPath.find_last_of("/\\") + 1);
    std::string fullImagePath = baseDir + imagePath;

    int width, height, nrComponents;
    unsigned char* imageData = stbi_load(fullImagePath.c_str(), &width, &height, &nrComponents, STBI_rgb_alpha);
    if (!imageData) {
        std::cerr << "Failed to load texture: " << fullImagePath << std::endl;
        return 0;
    }

    GLenum format = (nrComponents == 1) ? GL_RED :
                    (nrComponents == 3) ? GL_RGB :
                    (nrComponents == 4) ? GL_RGBA : GL_RGB;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::cout << "Loading texture: " << imagePath << " with ID: " << textureID << std::endl;

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(imageData);

    return textureID;
}
// #################################   END ######################################

inline std::map<std::string, GLuint> loadTextureForMaterial(const json& gltf, int materialIndex, const std::string& gltfPath) {
    std::map<std::string, GLuint> textures;

    if (materialIndex < 0 || materialIndex >= gltf["materials"].size()) return textures;
    const auto& material = gltf["materials"][materialIndex];

    // PBR Metallic-Roughness Textures
    if (material.contains("pbrMetallicRoughness")) {
        const auto& pbr = material["pbrMetallicRoughness"];

        if (pbr.contains("baseColorTexture")) {
            int index = pbr["baseColorTexture"]["index"];
            int imageIndex = gltf["textures"][index]["source"];
            textures["baseColor"] = loadTextureFromImageIndex(gltf, imageIndex, gltfPath);
            std::cout << " This model contain baseColor texture" << std::endl;
        }
        else {
            std::cout << " This model dose not contain baseColor texture" << std::endl;
        }

        if (pbr.contains("metallicRoughnessTexture")) {
            int index = pbr["metallicRoughnessTexture"]["index"];
            int imageIndex = gltf["textures"][index]["source"];
            textures["metallicRoughnessMap"] = loadTextureFromImageIndex(gltf, imageIndex, gltfPath);
            std::cout << " This model contain metallicRoughnessMap texture" << std::endl;
        }
        else {
            std::cout << " This model dose not contain metallicRoughnessTexture texture" << std::endl;
        }
        
        // Normal Map
        if (material.contains("normalTexture")) {
            int index = material["normalTexture"]["index"];
            int imageIndex = gltf["textures"][index]["source"];
            textures["normalMap"] = loadTextureFromImageIndex(gltf, imageIndex, gltfPath);
            std::cout << " This model contain normalMap texture" << std::endl;
        }
        else {
            std::cout << " This model dose not contain narmalMap  texture" << std::endl;
        }

        // Occlusion Map
        if (material.contains("occlusionTexture")) {
            int index = material["occlusionTexture"]["index"];
            int imageIndex = gltf["textures"][index]["source"];
            textures["occlusionMap"] = loadTextureFromImageIndex(gltf, imageIndex, gltfPath);
            std::cout << " This model contain occlusionMap texture" << std::endl;
        }
        else {
            std::cout << " This model dose not contain occlutionMap texture" << std::endl;
        }

        // Emissive Map
        if (material.contains("emissiveTexture")) {
            int index = material["emissiveTexture"]["index"];
            int imageIndex = gltf["textures"][index]["source"];
            textures["emissiveMap"] = loadTextureFromImageIndex(gltf, imageIndex, gltfPath);
            std::cout << " This model an contain emissiveMap texture" << std::endl;
        }
        else {
            std::cout << " This model dose not contain emissiveMap texture" << std::endl;
        }

    } 
    return textures;
}

inline void DestroyGLTFMesh(GLTFMesh& mesh) {
    for (auto& subMesh : mesh.submeshes) {
        if (subMesh.gltfEBO) glDeleteBuffers(1, &subMesh.gltfEBO);
        if (subMesh.gltfVBO) glDeleteBuffers(1, &subMesh.gltfVBO);
        if (subMesh.gltfVAO) glDeleteVertexArrays(1, &subMesh.gltfVAO);
        if (subMesh.textureID) glDeleteTextures(1, &subMesh.textureID);
    }
    mesh.submeshes.clear();
}


