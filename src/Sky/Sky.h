#pragma once
#include "stb/stb_image.h"
#include "../Headers/Config.h"
#include "../Headers/GlobalVars.h"

#include "../Ecs/BaseModel.h"
#include <filesystem>
namespace fs = std::filesystem;

struct SkyTexture {
    GLuint id;
    //GLuint textureID;
    std::string path;
    GLuint frontFaceTexID;
};

class LoadSkybox : public BaseModel {

public:
	unsigned int skyTextureID;
    unsigned int sky_VAO, sky_VBO, sky_EBO;
    unsigned int skyMapTexture;

	LoadSkybox(int idx, const std::string& name, int SkyIdx) {
		index = idx;
		objectName = name;
		objectIndex = SkyIdx;
		objectTypeID = SKY_BOX;

        float SkyBoxVertices[] =
        {
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f
        };

        unsigned int SkyBoxIndices[] =
        {
            // Right
            1, 2, 6,
            6, 5, 1,
            // Left
            0, 4, 7,
            7, 3, 0,
            // Top
            4, 5, 6,
            6, 7, 4,
            // Bottom
            0, 3, 2,
            2, 1, 0,
            // Back
            0, 1, 5,
            5, 4, 0,
            // Front
            3, 7, 6,
            6, 2, 3
        };

        glGenVertexArrays(1, &sky_VAO);
        glGenBuffers(1, &sky_VBO);
        glGenBuffers(1, &sky_EBO);
        glBindVertexArray(sky_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, sky_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(SkyBoxVertices), &SkyBoxVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SkyBoxIndices), &SkyBoxIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FLOAT, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Creat a cubemap for the skybox
		glGenTextures(1, &skyTextureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyTextureID);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// this stuff is important to prevent seames
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		//Cycles through the textures and fix them to the cubemap object
		
	}
    void DrawSky() {
        
        glBindVertexArray(sky_VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

// Utility to copy one face from source to destination
inline unsigned char* extract_face(const unsigned char* src, int srcWidth, int srcHeight, int channels,
    int faceRow, int faceCol, int faceSize = 512) {
    unsigned char* face = new unsigned char[faceSize * faceSize * channels];
    for (int y = 0; y < faceSize; ++y) {
        for (int x = 0; x < faceSize; ++x) {
            int srcX = faceCol * faceSize + x;
            int srcY = faceRow * faceSize + y;
            for (int c = 0; c < channels; ++c) {
                face[(y * faceSize + x) * channels + c] =
                    src[(srcY * srcWidth + srcX) * channels + c];
            }
        }
    }
    return face;
}

inline std::vector<SkyTexture> loadSkyTextureFromFolder(const std::string& folderPath) {
    std::vector<SkyTexture> sky_textures;

    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (!entry.is_regular_file()) continue;
        std::string ext = entry.path().extension().string();
        if (ext != ".png" && ext != ".jpg" && ext != ".bmp") continue;

        std::string imagePath = entry.path().string();
        int width, height, channels;
        unsigned char* data = stbi_load(imagePath.c_str(), &width, &height, &channels, 0);
        if (!data) {
            std::cerr << "Failed to load image: " << imagePath << "\n";
            continue;
        }

        const int faceSize = 512;
        std::vector<std::pair<int, int>> facePositions = {
            {0, 1}, // Top
            {1, 0}, // Left
            {1, 1}, // Front
            {1, 2}, // Right
            {1, 3}, // Back
            {2, 1}  // Bottom
        };

        GLenum faceTargets[6] = {
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
        };

        GLuint cubeMapID;
        glGenTextures(1, &cubeMapID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);

        GLuint previewTexID = 0;

        for (int i = 0; i < 6; ++i) {
            auto [row, col] = facePositions[i];
            unsigned char* face = extract_face(data, width, height, channels, row, col, faceSize);

            // Upload to cubemap
            glTexImage2D(faceTargets[i], 0,
                channels == 4 ? GL_RGBA : GL_RGB,
                faceSize, faceSize, 0,
                channels == 4 ? GL_RGBA : GL_RGB,
                GL_UNSIGNED_BYTE, face);

            // Also create preview texture from FRONT face
            if (i == 2) { // Front face (index 2)
                glGenTextures(1, &previewTexID);
                glBindTexture(GL_TEXTURE_2D, previewTexID);
                glTexImage2D(GL_TEXTURE_2D, 0,
                    channels == 4 ? GL_RGBA : GL_RGB,
                    faceSize, faceSize, 0,
                    channels == 4 ? GL_RGBA : GL_RGB,
                    GL_UNSIGNED_BYTE, face);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }

            delete[] face;
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        stbi_image_free(data);

        sky_textures.push_back({ cubeMapID, imagePath, previewTexID });
    }

    return sky_textures;
}


class SkyEditor : public BaseModel {
public:
    std::vector<SkyTexture> skyTexture;
    bool SkyLoaded = false;
    bool dialogType = true;

    void Render(std::vector<std::unique_ptr<BaseModel>>& selectedData) {

        if (ShowSkyEditor) { // if this is true open the editor window
            ImGui::GetStyle().WindowRounding = 12.0f;
            ImGui::Begin("Sky Editor", &ShowSkyEditor);
            ImGui::InputText("sky Name", nameBuffer, IM_ARRAYSIZE(nameBuffer));
            ImGui::TextColored(COLOR_LIGHTBLUE, ICON_FA_EDIT " Sky Editor");
            ImGui::SeparatorText("Sky Editor");

            
            // put an if in here
            if (dialogType) {
                ImGui::SeparatorText(" Sky Picker");
                ImGui::TextColored(COLOR_LIGHTBLUE, ICON_FA_IMAGE " Available Sky Textures");

                if (!SkyLoaded) {                            // this is the folder
                    skyTexture = loadSkyTextureFromFolder("Textures/Skybox/NewSky/");
                    SkyLoaded = true;
                }

                int columns = 6;
                int count = 0;
                ImGui::BeginChild("TextureGrid", ImVec2(0, 300), true); // scrollable
               
                for (const auto& sky : skyTexture) {
                    ImGui::PushID(sky.id);
                    if (ImGui::ImageButton((void*)(intptr_t)sky.frontFaceTexID, ImVec2(64, 64))) {
                        objectUpdateIndex = SelectedDataManager::Instance().GetSelectedData()->objectIndex;
                        if (objectUpdateIndex != -1) {

                            ObjectVector[objectUpdateIndex]->textureID = sky.id;
                            creatSkyTex = sky.id;
                            ShouldUpdateSkyBox = true;

                            std::cout << "SkyBox TextureID is: " << textureID <<
                                SelectedDataManager::Instance().GetSelectedData()->objectIndex << " Type " << SKY_BOX << std::endl;
                            
                        }
                    }
                    ImGui::PopID();

                    if (++count % columns != 0) ImGui::SameLine();
                }

                ImGui::EndChild();
            }

            // Update button                           
            if (ImGui::Button("Update")) { // Update the object's name 

                // Check if nameBuffer is not empty
                //if (strlen(nameBuffer) > 0) {
                //    if (SelectedDataManager::Instance().GetSelectedData() != nullptr) {
                //        SelectedDataManager::Instance().GetSelectedData()->objectName = std::string(nameBuffer);
                //        // THIS is what we are working on
                //        if (SelectedDataManager::Instance().GetSelectedData()->objectTypeID != -1) {


                //            switch (SelectedDataManager::Instance().GetSelectedData()->objectTypeID) {
                //            case 30: // SkyBox 
                //                ShouldUpdateSkyBox = true;
               
                //                std::cout << "SkyBox Object update index set to: " <<
                //                    SelectedDataManager::Instance().GetSelectedData()->objectIndex << " Type " << SKY_BOX << std::endl;
                //                break;

                //            case 31: // Sky Hemisphere              
                //                ShouldUpdateCube = true;
                //                std::cout << "SkyBox Object update index set to: " <<
                //                    SelectedDataManager::Instance().GetSelectedData()->objectIndex << " Type " << SKY_BOX_HEM << std::endl;
                //                
                //                break;
                //            case 32: // Sky HDRI
                //                ShouldUpdatePlane = true;
                //                std::cout << "SkyBox Object update index set to: " <<
                //                    SelectedDataManager::Instance().GetSelectedData()->objectIndex << " Type " << SKY_BOX_HDRI << std::endl;
                //                break;
                //            case 33: // not in use
                //                break;

                //                
                //            }
                //        }

                //        objectUpdateIndex = SelectedDataManager::Instance().GetSelectedData()->objectIndex; // Ensure this is set correctly

                //        std::cout << "Object update index set to: " << objectUpdateIndex << std::endl;

                //        std::cout << "Selected " << SelectedDataManager::Instance().GetSelectedData()->objectName << " Index " <<
                //            SelectedDataManager::Instance().GetSelectedData()->objectIndex << std::endl;
                //    }


                //}
                //ShouldAddSkyBox = true;

                ShowSkyEditor = false;

                creatMap = 0;

            }
            ImGui::SameLine();
            if (ImGui::Button("Close Editor")) {
                ShowSkyEditor = false;
            }

            ImGui::End();
        }

    }

};

//inline std::vector<SkyTexture> loadSkyTextureFromFolder(const std::string& SkyPath) {
//    std::vector<SkyTexture> sky_texture;
//    
//    // Load the packed skybox texture
//    int width, height, nrChannels;
//    unsigned char* data = stbi_load(SkyPath.c_str(), &width, &height, &nrChannels, 0);
//    if (!data) {
//        std::cerr << "Failed to load skybox image: " << SkyPath << std::endl;
//        return sky_texture;
//    }
//
//    const int faceSize = 512;
//
//    // Define face positions
//    std::vector<std::pair<int, int>> facePositions = {
//        {0, 1}, // Top
//        {1, 0}, // Left
//        {1, 1}, // Front
//        {1, 2}, // Right
//        {1, 3}, // Back
//        {2, 1}  // Bottom
//    };
//
//    std::vector<std::string> faceNames = {
//        "Top", "Left", "Front", "Right", "Back", "Bottom"
//    };
//
//    for (int i = 0; i < 6; ++i) {
//        auto [row, col] = facePositions[i];
//        unsigned char* faceData = extract_face(data, width, height, nrChannels, row, col, faceSize);
//
//        GLuint texID;
//        glGenTextures(1, &texID);
//        glBindTexture(GL_TEXTURE_2D, texID);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, faceSize, faceSize, 0,
//            nrChannels == 4 ? GL_RGBA : GL_RGB,
//            GL_UNSIGNED_BYTE, faceData);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//        sky_texture.push_back({ texID, faceNames[i] });
//
//        delete[] faceData;
//    }
//
//    stbi_image_free(data);
//    return sky_texture;
//}



//inline std::vector<SkyTexture> loadSkyTextureFromFolder(const std::string& SkyPath) {
//    std::vector<SkyTexture> sky_texture;
//    // load all sky textures from a folder so we can select which to use
//    for (const auto& entry : fs::directory_iterator(SkyPath)) {
//        if (entry.is_regular_file()) {
//            std::string ext = entry.path().extension().string();
//            if (ext == ".png" || ext == ".jpg" || ext == ".bmp") {
//                GLuint id = loadSaveTerrainTexture(entry.path().string());
//                sky_texture.push_back({ id, entry.path().string() });
//            }
//        }
//    }
//
//    for (unsigned int i = 0; i < 6; i++)
//    {
//        int width, height, nrChannels;
//        // I need to load the texture and "Texture/skybox/NewSky/cubemap_01.jpg
//        unsigned char* data = stbi_load(SkyPath.c_str(), &width, &height, &nrChannels, 0);
//        // the texture is 2048 x 1536 and each of the 6 faces are 512 x 512.
//        // I need to get the part of the texture with the X in and set them to Top, Left, Front, Right, Back and Bottum
//        //  512    512   512   512
//        // -------------------------
//        // |     |  X  |     |     | 512
//        // -------------------------
//        // |  X  |  X  |  X  |  X  | 512
//        // -------------------------
//        // |     |  X  |     |     | 512
//        // -------------------------
//
//
//
//    }
//    return sky_texture;
//
//}
