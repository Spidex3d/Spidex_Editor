#include "EntityNodes.h"
#include "../Windows/spx_FileDialog.h"
#include "../Object_loader\objLoader.h"

#include "../Object_loader/gltf.h"

unsigned int loadTexture(const std::string& filePath);


EntityNodes* EntityNodes::Instance() {
    static EntityNodes component;
    return &component;
}

void EntityNodes::Initialize()
{
    BaseModel* selectedData = nullptr;  // Define the external variable
        
    // Add The Default editor grid on start up                       TO DO ADD THIS TO SETTINGS
    ObjectVector.push_back(std::make_unique<MainGrid>(0, "Main Grid", grid_square, grid_size));
   
}

void EntityNodes::ObjectEditor(std::vector<std::unique_ptr<BaseModel>>& selectedData)
{
    if (showObjectEditor) { // if this is true open the editor window
        ImGui::GetStyle().WindowRounding = 12.0f;
        ImGui::Begin("Mesh Object Editor", &showObjectEditor);
        ImGui::InputText("Object Name", nameBuffer, IM_ARRAYSIZE(nameBuffer));
        ImGui::TextColored(COLOR_LIGHTBLUE, ICON_FA_EDIT " Mesh Editor");
        ImGui::SeparatorText("Object Editor");

        ImGui::BeginTable("Edit Table", 1, ImGuiTableFlags_Reorderable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders);

        ImGui::TableNextColumn();
       
        // Retrieve the current position and scale from the selected object
        ImGui::DragFloat3("##pos", object_Pos, 0.0f, 0.0f, 0.0f);       
        ImGui::SameLine();
        if (ImGui::Button("Reset_Position")) {
            object_Pos[0] = 0.0f, object_Pos[1] = 0.0f, object_Pos[2] = 0.0f;
        }
        
        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        ImGui::DragFloat3("##Scale", object_Scale, 0.0f, 0.0f, 0.0f);
        ImGui::SameLine();
        if (ImGui::Button("Reset Scale")) {
            object_Scale[0], object_Scale[1], object_Scale[2];
        }

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        
        ImGui::DragFloat3("##Rot", object_Rotation, 0.0f, 0.0f, 0.0f);
        ImGui::SameLine();
        if (ImGui::Button("Reset Rotation")) {
            object_Rotation[0] = 0.0f, object_Rotation[1] = 0.0f, object_Rotation[2] = 0.0f;
        }

        ImGui::EndTable();
        
        // put an if in here
        if (dialogType) { //  IsTexture must be true to show button
        ImGui::TextColored(COLOR_LIGHTBLUE, ICON_FA_IMAGE " Texture Editor");
        ImGui::SeparatorText(" Texture Editor");
       
        // load the texture
        spx_FileDialog openDialog;
            if (ImGui::Button("Set New Texture")) {
                std::string myTexturePath = openDialog.openFileDialog();
                if (!myTexturePath.empty()) {
                    std::cout << "Texture path selected: " << myTexturePath << std::endl;
                    objectUpdateIndex = SelectedDataManager::Instance().GetSelectedData()->objectIndex; // just added
                    if (objectUpdateIndex != -1) {
                        std::cout << "Updating texture for cube index: " << objectUpdateIndex << std::endl;
                        ObjectVector[objectUpdateIndex]->textureID = loadTexture(myTexturePath);

                        std::cout << "New texture ID: " << ObjectVector[objectUpdateIndex]->textureID << std::endl;

                        creatMap = loadTexture((myTexturePath).c_str()); // this is the image on the object edit window
                    }
                    else {
                        std::cout << "objectUpdateIndex is not set correctly." << std::endl;
                    }
                }
                else {
                    std::cout << "No texture path selected." << std::endl;
                }
            }
        }

        if (creatMap != 0) {
            ImGui::Text("Your selected Texture");
            ImGui::Image((void*)(intptr_t)creatMap, ImVec2(65, 65));
        }
        else {
           // std::cout << "No texture loaded." << std::endl;
        }
        

        // Update button                           
        if (ImGui::Button("Update")) { // Update the object's name 

            // Check if nameBuffer is not empty
            if (strlen(nameBuffer) > 0) {
                if (SelectedDataManager::Instance().GetSelectedData() != nullptr) {
                    SelectedDataManager::Instance().GetSelectedData()->objectName = std::string(nameBuffer);
                    // THIS is what we are working on
                    if (SelectedDataManager::Instance().GetSelectedData()->objectTypeID != -1) {


                        switch (SelectedDataManager::Instance().GetSelectedData()->objectTypeID) {
                        case 0: // Grid open the settings window
                            //show_settings_window = true;
                            break;
                        
                        case 1: // Cube                     
                            ShouldUpdateCube = true;
                            std::cout << "Cube Object update index set to: " <<
                                SelectedDataManager::Instance().GetSelectedData()->objectIndex << " Type " << OBJ_CUBE << std::endl;
                            break;
                        case 2: // Plane
                            ShouldUpdatePlane = true;

                            std::cout << "plane Object update index set to: " <<
                                SelectedDataManager::Instance().GetSelectedData()->objectIndex << " Type " << OBJ_PLANE << std::endl;
                            break;
                        case 3: // Circle
                            
                            break;
                        case 4: // Line
                            break;
                        case 5: // OBJ_SPHERE
                            ShouldUpdateSphere = true;
                            std::cout << "Cube Object update index set to: " <<
                                SelectedDataManager::Instance().GetSelectedData()->objectIndex << " Type " << OBJ_SPHERE << std::endl;
                            break;
                        case 6: // OBJ_CYLINDER
                            break;
                        case 7: // OBJ_TORUS
                            break;
                        case 8: //  Grid not the editor grid !
                            break;
                        case 9: //  Cone                        
                            break;
                        case 10: // pyramid
                            ShouldUpdatePyramid = true;
                            break;
                        case 11: //  Triangel                        
                            break;
                        case 12: // Obj Models
                            ShouldUpdateObjModel = true;
                            break;
                        case 13: // Not in use
                             break;
                        case 14: // glTF Models file
                            ShouldUpdateObjModel = true;
                            break;               
                        case 15: // Not in use
                            break;
                        case 16: // Not in use
                            break;
                        case 17: // Not in use
                            break;
                        case 18: // Not in use
                            break;
                        case 19: // Not in use
                            break;
                        case 20: // sun light LIGHT_SUN
                            ShouldUpdateLight = true;
                            break;

                        case 21: // Point Light
                            break;
                        case 22: // Spot Light
                            break;
                        case 23: // Area Light
                            break;
                        case 24: // Not in use
                            break;
                        case 25: // Floor
                            ShouldUpdateFloor = true;  
                            break;
                        case 26: // Terrain
                            //ShouldAddTerrain = true;
                            break;
                        case 27: // Water
                            //ShouldAddWater = true;
                            break;

                        }
                    }


                    objectUpdateIndex = SelectedDataManager::Instance().GetSelectedData()->objectIndex; // Ensure this is set correctly

                    std::cout << "Object update index set to: " << objectUpdateIndex << std::endl;

                    std::cout << "Selected " << SelectedDataManager::Instance().GetSelectedData()->objectName << " Index " <<
                        SelectedDataManager::Instance().GetSelectedData()->objectIndex << std::endl;
                } 
                
            }
                showObjectEditor = false;

                
                creatMap = 0;
               
        }
       
        ImGui::End();
    }
    
}

std::vector<std::unique_ptr<BaseModel>>& EntityNodes::GetModels()
{
    return ObjectVector;
}

void EntityNodes::EntityManagmentSystem(std::vector<std::unique_ptr<BaseModel>>& ObjectVector,
    int& currentIndex, int& index, int& objectIndex, int& indexTypeID) {
    ImGui::Begin("Entity Management System"); // start of the window

    if (ImGui::BeginTabBar("##Main", ImGuiTabBarFlags_None))
    {
        if (ImGui::BeginTabItem("Scene Lab"))
        {
            ImGui::SeparatorText("Scene Collection");

            auto flags = ImGuiTreeNodeFlags_DefaultOpen;
            if (ImGui::TreeNodeEx("Editor Scene", flags)) {

                for (const auto& data : ObjectVector) {
                    
                    ImGuiTreeNodeFlags nodeFlags = flags | (SelectedDataManager::Instance().GetSelectedData() == data.get() ? ImGuiTreeNodeFlags_Selected : 0);

                    bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)data->objectIndex, nodeFlags,
                        "  %s : %d : Type ID : %d",
                       // %d : %s : %d : Type ID : %d",
                        //"Object: %d : %s : %d : Type ID : %d",
                        data->objectName.c_str(), data->objectIndex, data->objectTypeID);
                       // data->index, data->objectName.c_str(), data->objectIndex, data->objectTypeID);
                    // #####
                   
                    // #####
                    if (ImGui::IsItemClicked()) {
                        SelectedDataManager::Instance().SetSelectedData(data.get());
                        data->isSelected = true;
                        
                        std::cout << "Data Selected was " << data->objectName.c_str() << " : " << data->objectIndex << std::endl;
                    }

                    if (ImGui::IsItemHovered()) {
                        // Perform actions when the node is hovered
                        ImGui::SetTooltip("Right click to Edit %s | %d", data->objectName.c_str(), data->objectIndex);
                        
                    }

                    if (nodeOpen) {
                        //this->onRightClick(data->objectIndex); // Pass a unique identifier
                        this->onRightClick(data->index); // Pass a unique identifier

                        if (ImGui::BeginPopup(("NodePopup" + std::to_string(data->index)).c_str())) {
                            ImGui::TextColored(COLOR_LIGHTBLUE, ICON_FA_EDIT " ENTITY");
                            ImGui::Separator(); // Draw a line
                            if (ImGui::Selectable(ICON_FA_PEN_ALT " Edit")) {
                                SelectedDataManager::Instance().SetSelectedData(data.get());

                                // Copy the current name to the buffer
                                 strncpy_s(nameBuffer, data->objectName.c_str(), sizeof(nameBuffer));
                                 nameBuffer[sizeof(nameBuffer) - 1] = '\0';
                                
                               
                                std::cout << "Object Selected To Edit " << data->objectName.c_str() << " : " << data->objectIndex << std::endl;
                                
                                switch (data.get()->objectTypeID) {
                                case 0: // Grid
                                    break;
                                case 1: // cube
                                    showObjectEditor = true;
                                    IsTexture = true;

                                    std::cout << "Data Selected  is a Cube " << data->objectName.c_str() << " : " << data->objectIndex << std::endl;
                                    LogInternals::Instance()->Debug("Data Selected  is a Cube");
                                    break;
                                case 2: // plane
                                    showObjectEditor = true;
                                    LogInternals::Instance()->Debug("Data Selected  is a plane");
                                    break;
                                case 3:
                                    break;
                                case 4: //
                                   
                                    break;
                                case 5:
                                    showObjectEditor = true;
                                    IsTexture = true;
                                    break;
                                case 6:
                                    break;
                                case 7:
                                    break;
                                case 8:
                                    showObjectEditor = true;
                                    break;
                                case 9:
                                    showObjectEditor = true;
                                    break;
                                case 10:
                                    showObjectEditor = true;
                                    break;
                                case 11: // pyramid
                                    showObjectEditor = true;
                                    LogInternals::Instance()->Debug("Data Selected  is a plane");
                                    break;
                                case 12: // 0bj file
                                    showObjectEditor = true;
                                    dialogType = false; // set to false and not show the add texture button
                                    LogInternals::Instance()->Debug("Data Selected  is a obj file");
                                    break;
                                case 13:
                                    break;
                                case 14:
                                    showObjectEditor = true;
                                    dialogType = false; // set to false and not show the add texture button
                                    LogInternals::Instance()->Debug("Data Selected  is a glTF file");
                                    break;
                                case 15: // Not in use
                                    break;
                                case 16: // Not in use
                                    break;
                                case 17: // Not in use
                                   break;
                                case 18: // Not in use
                                   break;
                                case 19: // Not in use
                                   break;
                                 case 20: // sun light LIGHT_SUN
                                     showObjectEditor = true;
                                    break;
                                 case 21: // Not in use
                                    break;
                                 case 22: // Not in use
                                    break;
                                 case 23: // Not in use
                                    break;
                                 case 24: // Not in use
                                    break;
                                 case 25: // Not in use
                                     showObjectEditor = true;
                                    break;
                                 case 26: // Not in use
                                     showObjectEditor = true;
                                    break;
                                 case 27: // Not in use
                                     showObjectEditor = true;
                                    break;


                                default:
                                    std::cout << "Data Selected Something Else " << data->objectName.c_str() << " : " << data->objectIndex << std::endl;
                                    break;

                                }                                                                              
                            
                            
                            }
                            if (ImGui::Selectable(ICON_FA_PLUS " New")) {
                                // open a window or popup to select the object
                                std::cout << "Object To Add "  << std::endl;
                            }
                            if (ImGui::Selectable(ICON_FA_COPY " Clone")) {

                                std::cout << "Data Selected To Clone " << data->objectName.c_str() << " : " << data->objectIndex << std::endl;
                            }
                            if (ImGui::Selectable(ICON_FA_TRASH_ALT " Delete")) {
                                // open window to ask if your sure
                                std::cout << "Data Selected To Delete " << data->objectName.c_str() << " : " << data->objectIndex << std::endl;
                            }

                            ImGui::EndPopup();
                        }

                        ImGui::TreePop();
                                                
                    }
                    
            EntityNodes::Instance()->DrawSelectionBox(ObjectVector);
                }
                ImGui::TreePop();
                                
            }      

            ImGui::EndTabItem();

        }

        if (ImGui::BeginTabItem("Camera Lab"))
        {
            ImGui::Text("ID: Camera Lab");
            ImGui::Text("Spidex Engine New Camera Lab", nullptr);

            if (ImGui::CollapsingHeader(ICON_FA_VIDEO" Camera Settings", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::BeginTable("Test Table", 1, ImGuiTableFlags_Reorderable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders);

                ImGui::TableNextColumn();

                ImGui::SameLine();
                if (ImGui::Button("Reset")) {
                    cam_pos_val[0] = 0.0f, cam_pos_val[1] = 0.0f, cam_pos_val[2] = 2.0f;
                }
                ImGui::SameLine();

                ImGui::DragFloat3("Position", cam_pos_val, 1.0f, 1.0f, 2.0f);
                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                ImGui::SameLine();
                if (ImGui::Button("Reset")) {
                    cam_rot_val[0] = 0.0f, cam_rot_val[1] = 0.0f, cam_rot_val[2] = 0.0f;
                }
                ImGui::SameLine();
                ImGui::DragFloat3("Rotation", cam_rot_val, 1.0f, 1.0f, 1.0f);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                ImGui::SameLine();
                if (ImGui::Button("Reset")) {
                    cam_scale_val[0] = 1.0f, cam_scale_val[1] = 1.0f, cam_scale_val[2] = 1.0f;
                }// 1,1,1
                ImGui::SameLine();
                ImGui::DragFloat3("Scale", cam_scale_val, 1.0f, 1.0f, 1.0f);

                //ImGui::Selectable(label, &selected);

                ImGui::EndTable();
            }
           
            ImGui::SeparatorText("Scene Collection");
            // Test canera list

                auto flags = ImGuiTreeNodeFlags_DefaultOpen;
                if (ImGui::TreeNodeEx("Cameras", flags)) {

                   
                    // Camera Main
                    ImGuiTreeNodeFlags nodeFlagsMain = flags | ImGuiTreeNodeFlags_Leaf;
                    ImGui::TreeNodeEx("Camera Main", nodeFlagsMain);
                    if (ImGui::IsItemClicked()) {
                        // Handle the selection of Camera Main
                        std::cout << "Camera Main selected" << std::endl;
                    }
                    ImGui::TreePop();

                    // Camera Top
                    ImGuiTreeNodeFlags nodeFlagsTop = flags | ImGuiTreeNodeFlags_Leaf;
                    ImGui::TreeNodeEx("Camera Top", nodeFlagsTop);
                    if (ImGui::IsItemClicked()) {
                        // Handle the selection of Top Left
                        std::cout << "Camera Top selected" << std::endl;
                    }
                    ImGui::TreePop();

                    // Camera Left
                    ImGuiTreeNodeFlags nodeFlagsLeft = flags | ImGuiTreeNodeFlags_Leaf;
                    ImGui::TreeNodeEx("Camera Left", nodeFlagsLeft);
                    if (ImGui::IsItemClicked()) {
                        // Handle the selection of Camera Left
                        std::cout << "Camera Left selected" << std::endl;
                    }
                    ImGui::TreePop();

                    // Camera Right
                    ImGuiTreeNodeFlags nodeFlagsRight = flags | ImGuiTreeNodeFlags_Leaf;
                    ImGui::TreeNodeEx("Camera Right", nodeFlagsRight);
                    if (ImGui::IsItemClicked()) {
                        // Handle the selection of Camera Right
                        std::cout << "Camera Right selected" << std::endl;
                    }
                    ImGui::TreePop();

                ImGui::TreePop();
                   
                }          

            // End Test camera list

            ImGui::EndTabItem();

        }

        if (ImGui::BeginTabItem("Render Lab"))
        {
            ImGui::Text("ID: Render Lab");
            ImGui::Text("Spidex Engine New Render Lab", nullptr);

            if (ImGui::Button("Render Image")) {
                std::cout << "Render The Image on a new form" << std::endl;
            }

            ImGui::EndTabItem();

        }

        ImGui::EndTabItem();
    }
    
    ImGui::End();
}

void EntityNodes::EntityProperties()
{
   
}
        // Render Grid
void EntityNodes::RenderGrid(const glm::mat4& view, const glm::mat4& projection,
    std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Gridobjidx)
{
    ShaderManager::defaultGridShader->Use();
    ShaderManager::defaultGridShader->setMat4("projection", projection);
    ShaderManager::defaultGridShader->setMat4("view", view);

    for (const auto& model : ObjectVector) {  // ObjectVector = 1
        if (auto* grid = dynamic_cast<MainGrid*>(model.get())) {

            modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(20.0f, 0.0f, 20.0f));
            ShaderManager::defaultGridShader->setMat4("model", modelMatrix);

            grid->DrawGrid();
        }
    }
}

// ####   Render Scene
void EntityNodes::RenderScene(const glm::mat4& view, const glm::mat4& projection,
    std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, Shader& shader, Camera& camera)
{
   
    EntityNodes::RenderCube(view, projection, ObjectVector, currentIndex, Cubeobjidx);
    EntityNodes::RenderSphere(view, projection, ObjectVector, currentIndex, Sphereobjidx);
    EntityNodes::RenderTriangle(view, projection, ObjectVector);
    EntityNodes::RenderPlane(view, projection, ObjectVector, currentIndex, Planeobjidx);
    EntityNodes::RenderPyramid(view, projection, ObjectVector, currentIndex, Pyramidobjidx);
    // ############################### Models #####################################
    EntityNodes::RenderObjFiles(view, projection, ObjectVector, currentIndex, ModleObjidx);
    EntityNodes::RendergltfFiles(view, projection, ObjectVector, currentIndex, glTFModelIndex, shader, camera);
    // ############################### Lighting #####################################
    EntityNodes::RenderSunLightSprite(view, projection, ObjectVector, currentIndex, Lightidx);
    EntityNodes::RenderPointLightSprite(view, projection, ObjectVector, currentIndex, Lightidx);
   
    // ############################### Terrain #####################################
    EntityNodes::RenderTerrainFloor(view, projection, ObjectVector, currentIndex, TerrainIdx, camera);
}
void EntityNodes::RendergltfFiles(const glm::mat4& view, const glm::mat4& projection,
    std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& glTFModelIndex, Shader& shader, Camera& camera)
{
    if (ShouldAddglTFModel) {
        glTFModelIndex = ObjectVector.size();

        std::unique_ptr<Gltf> newGLTFModel = std::make_unique<Gltf>(currentIndex++,
            "New gltf File", glTFModelIndex);

        spx_FileDialog openModelDialog;
        std::string modelPath = openModelDialog.openFileDialog();
        // Add some error handeling here
        if (modelPath.empty()) {
            std::cerr << "No file selected or invalid file path." << std::endl;
            ShouldAddglTFModel = false;
            return;
        }
        // Load the glTF model
        std::string binPath = modelPath;
        binPath.replace(binPath.find(".gltf"), 5, ".bin");

        std::cout << "Loading Gltf Model from: " << modelPath << std::endl;

        if (!newGLTFModel->Load(modelPath, binPath)) {
            std::cerr << "Failed to load gltf model: " << std::endl;
            ShouldAddglTFModel = false;
            return;
        }

        switch (glTFModelIndex) {
        case 1:
            newGLTFModel->position = glm::vec3(0.0f, 0.0f, 0.0f);
            newGLTFModel->scale = glm::vec3(1.0f, 1.0f, 1.0f);
            break;

        default:
            newGLTFModel->position = glm::vec3(1.0f, 0.0f, 0.0f);
            newGLTFModel->scale = glm::vec3(0.5f, 0.5f, 0.5f);
            break;
        }
              
       newGLTFModel->modelMatrix = glm::translate(glm::mat4(1.0f), newGLTFModel->position);
       newGLTFModel->modelMatrix = glm::scale(newGLTFModel->modelMatrix, newGLTFModel->scale);
       

        ObjectVector.push_back(std::move(newGLTFModel));

        ShouldAddglTFModel = false;

    }

    if (ShouldUpdateglTFModel) { // Edit and move obj model
        int selectedIndex = SelectedDataManager::Instance().GetSelectedData()->objectIndex;

        if (selectedIndex >= 0 && selectedIndex < ObjectVector.size()) {
            glm::vec3 newglTFModelPosition = glm::vec3(object_Pos[0], object_Pos[1], object_Pos[2]);
            ObjectVector[selectedIndex]->position = newglTFModelPosition;

            glm::vec3 newglTFModelScale = glm::vec3(object_Scale[0], object_Scale[1], object_Scale[2]);
            ObjectVector[selectedIndex]->scale = newglTFModelScale;

            ObjectVector[selectedIndex]->modelMatrix = glm::mat4(1.0f);
            ObjectVector[selectedIndex]->modelMatrix = glm::translate(ObjectVector[selectedIndex]->modelMatrix, newglTFModelPosition);
            ObjectVector[selectedIndex]->modelMatrix = glm::scale(ObjectVector[selectedIndex]->modelMatrix, newglTFModelScale);
        }

        ShouldUpdateglTFModel = false; // Reset the flag after Editing the gltf Model
    }

    for (const auto& model : ObjectVector) {
        // GLTFShader
        ShaderManager::defaultShader->setMat4("projection", projection);
        ShaderManager::defaultShader->setMat4("view", view);

        for (const auto& model : ObjectVector) {

            if (auto* gltfModel = dynamic_cast<Gltf*>(model.get())) {

                gltfModel->Draw(shader, camera);
            }
        }        
    }
}
void EntityNodes::RenderObjFiles(const glm::mat4& view, const glm::mat4& projection,
    std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& ModleObjidx)
{
         
    if (ShouldAddObjModel) {     
    ModleObjidx = ObjectVector.size();
    
    std::unique_ptr<objLoader> newObjModel = std::make_unique<objLoader>(currentIndex++, "New ObjFile", ModleObjidx);
   
    spx_FileDialog openModelDialog;
    std::string modelPath = openModelDialog.openFileDialog();
           
        if (newObjModel->Loadobj(modelPath)) {
            newObjModel->objModels();          
        }
        
        switch (ModleObjidx) {
        case 1:
            newObjModel->position = glm::vec3(0.0f, 0.0f, 0.0f);
            newObjModel->scale = glm::vec3(1.0f, 1.0f, 1.0f);
            break;

        default:
            newObjModel->position = glm::vec3(1.0f, 0.0f, 0.0f);
            newObjModel->scale = glm::vec3(0.5f, 0.5f, 0.5f);
            break;
        }

        newObjModel->modelMatrix = glm::translate(glm::mat4(1.0f), newObjModel->position);
        newObjModel->modelMatrix = glm::scale(newObjModel->modelMatrix, newObjModel->scale);

        ObjectVector.push_back(std::move(newObjModel));
      
        ShouldAddObjModel = false; // Reset the flag after adding the Obj Model
    }

    if (ShouldUpdateObjModel) { // Edit and move obj model
        int selectedIndex = SelectedDataManager::Instance().GetSelectedData()->objectIndex;

        if (selectedIndex >= 0 && selectedIndex < ObjectVector.size()) {
            glm::vec3 newObjModelPosition = glm::vec3(object_Pos[0], object_Pos[1], object_Pos[2]);
            ObjectVector[selectedIndex]->position = newObjModelPosition;

            glm::vec3 newObjModelScale = glm::vec3(object_Scale[0], object_Scale[1], object_Scale[2]);
            ObjectVector[selectedIndex]->scale = newObjModelScale;

            ObjectVector[selectedIndex]->modelMatrix = glm::mat4(1.0f);
            ObjectVector[selectedIndex]->modelMatrix = glm::translate(ObjectVector[selectedIndex]->modelMatrix, newObjModelPosition);
            ObjectVector[selectedIndex]->modelMatrix = glm::scale(ObjectVector[selectedIndex]->modelMatrix, newObjModelScale);
        }

        ShouldUpdateObjModel = false; // Reset the flag after Editing the Obj Model
    }
    // defaultShader
    for (const auto& model : ObjectVector) {
        ShaderManager::defaultShader->Use();
        ShaderManager::defaultShader->setMat4("projection", projection);
        ShaderManager::defaultShader->setMat4("view", view);

        //if (auto* objModel = dynamic_cast<objectModel*>(model.get())) {
        if (auto* objModel = dynamic_cast<objLoader*>(model.get())) {
           
            ShaderManager::defaultShader->setMat4("model", objModel->modelMatrix);
            
            objModel->objDrawModels();
        }
        else {
           
        }
    }
}

float posx = 0;
// #############   Render a Cube
void EntityNodes::RenderCube(const glm::mat4& view, const glm::mat4& projection,
    std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Cubeobjidx) {

     stbi_set_flip_vertically_on_load(true);    

    if (ShouldAddCube) { // so we add the cube
        Cubeobjidx = ObjectVector.size();
        std::unique_ptr<CubeModel> newCube = std::make_unique<CubeModel>(currentIndex++, "DefaultCube", Cubeobjidx);
        
        switch (Cubeobjidx) {
        case 1:
            newCube->position = glm::vec3(0.0f, 0.0f, 0.0f);
            newCube->scale = glm::vec3(1.0f, 1.0f, 1.0f);
            break;
        case 2:
            newCube->position = glm::vec3(1.5f, 0.0f, 0.0f);
            newCube->scale = glm::vec3(1.0f, 1.0f, 1.0f);
            break;

        case 3:
            newCube->position = glm::vec3(2.0f, 0.0f, 0.0f);
            newCube->scale = glm::vec3(1.0f, 1.0f, 1.0f);
            break;
        default:
            newCube->position = glm::vec3(posx, 2.0f, 0.0f);
            newCube->scale = glm::vec3(1.0f, 1.0f, 1.0f);
            posx += 1.5;
            break;
        }

       // newCube->modelMatrix = glm::mat4(1.0f);
       // newCube->modelMatrix = glm::translate(newCube->modelMatrix, newCube->position);
        newCube->modelMatrix = glm::translate(glm::mat4(1.0f), newCube->position);
        newCube->modelMatrix = glm::scale(newCube->modelMatrix, newCube->scale);

        newCube->textureID = loadTexture("Textures/default_1.jpg");

        ObjectVector.push_back(std::move(newCube));

        ShouldAddCube = false; // Reset the flag after adding the cube
    }
       
    if (ShouldUpdateCube) { // then we update the cube position and scale
               
        int selectedIndex = SelectedDataManager::Instance().GetSelectedData()->objectIndex;       
                
        if (selectedIndex >= 0 && selectedIndex < ObjectVector.size()) { // -1 to remove the grid
                                  
            glm::vec3 newCubePosition = glm::vec3(object_Pos[0], object_Pos[1], object_Pos[2]); // New position
            ObjectVector[selectedIndex]->position = newCubePosition;
           
            glm::vec3 newCubeScale = glm::vec3(object_Scale[0], object_Scale[1], object_Scale[2]); // New scale
            ObjectVector[selectedIndex]->scale = newCubeScale;

            //glm::mat4 newCubeRotation = glm::mat4(object_Rotation[0], object_Rotation[1], object_Rotation[2], object_Rotation[3]); // New Rotation
           // ObjectVector[index]->rotation = newCubeRotation;

                        
            ObjectVector[selectedIndex]->modelMatrix = glm::mat4(1.0f);
            ObjectVector[selectedIndex]->modelMatrix = glm::translate(ObjectVector[selectedIndex]->modelMatrix, newCubePosition);
            ObjectVector[selectedIndex]->modelMatrix = glm::scale(ObjectVector[selectedIndex]->modelMatrix, newCubeScale);
                        
        }
        
        ShouldUpdateCube = false;
        
    }
        
    for (const auto& model : ObjectVector) {
        ShaderManager::defaultShader->Use();
        ShaderManager::defaultShader->setMat4("projection", projection);
        ShaderManager::defaultShader->setMat4("view", view);


        if (auto* cube = dynamic_cast<CubeModel*>(model.get())) {
            modelMatrix = glm::mat4(1.0f);
            ShaderManager::defaultShader->setMat4("model", cube->modelMatrix);
        glActiveTexture(GL_TEXTURE0);

             glBindTexture(GL_TEXTURE_2D, cube->textureID);
             cube->DrawCube();
             glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}

void EntityNodes::RenderSphere(const glm::mat4& view, const glm::mat4& projection, std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Sphereobjidx)
{
    stbi_set_flip_vertically_on_load(true);

    if (ShouldAddSphere) { // so we add the cube
        Sphereobjidx = ObjectVector.size();
        std::unique_ptr<SphereModel> newSphere = std::make_unique<SphereModel>(currentIndex++, "DefaultSphere", Sphereobjidx);

        switch (Sphereobjidx) {
        case 1:
            newSphere->position = glm::vec3(0.0f, 0.0f, 0.0f);
            newSphere->scale = glm::vec3(1.0f, 1.0f, 1.0f);
            break;
        case 2:
            newSphere->position = glm::vec3(1.5f, 0.0f, 0.0f);
            newSphere->scale = glm::vec3(1.0f, 1.0f, 1.0f);
            break;

        case 3:
            newSphere->position = glm::vec3(2.0f, 0.0f, 0.0f);
            newSphere->scale = glm::vec3(1.0f, 1.0f, 1.0f);
            break;
        default:
            newSphere->position = glm::vec3(posx, 2.0f, 0.0f);
            newSphere->scale = glm::vec3(1.0f, 1.0f, 1.0f);
            posx += 1.5;
            break;
        }

        newSphere->modelMatrix = glm::translate(glm::mat4(1.0f), newSphere->position);
        newSphere->modelMatrix = glm::scale(newSphere->modelMatrix, newSphere->scale);

        newSphere->textureID = loadTexture("Textures/default_1.jpg");

        ObjectVector.push_back(std::move(newSphere));

        ShouldAddSphere = false; // Reset the flag after adding the cube
    }

    if (ShouldUpdateSphere) { // then we update the cube position and scale

        int selectedIndex = SelectedDataManager::Instance().GetSelectedData()->objectIndex;

        if (selectedIndex >= 0 && selectedIndex < ObjectVector.size()) { // -1 to remove the grid

            glm::vec3 newSpherePosition = glm::vec3(object_Pos[0], object_Pos[1], object_Pos[2]); // New position
            ObjectVector[selectedIndex]->position = newSpherePosition;

            glm::vec3 newSphereScale = glm::vec3(object_Scale[0], object_Scale[1], object_Scale[2]); // New scale
            ObjectVector[selectedIndex]->scale = newSphereScale;

            //glm::mat4 newCubeRotation = glm::mat4(object_Rotation[0], object_Rotation[1], object_Rotation[2], object_Rotation[3]); // New Rotation
           // ObjectVector[index]->rotation = newCubeRotation;

            ObjectVector[selectedIndex]->modelMatrix = glm::mat4(1.0f);
            ObjectVector[selectedIndex]->modelMatrix = glm::translate(ObjectVector[selectedIndex]->modelMatrix, newSpherePosition);
            ObjectVector[selectedIndex]->modelMatrix = glm::scale(ObjectVector[selectedIndex]->modelMatrix, newSphereScale);

        }

        ShouldUpdateSphere = false;

    }

    for (const auto& model : ObjectVector) {
        //ShaderManager::defaultShader->Use();
        ShaderManager::defaultShader->Use();
        ShaderManager::defaultShader->setMat4("projection", projection);
        ShaderManager::defaultShader->setMat4("view", view);

        if (auto* sphere = dynamic_cast<SphereModel*>(model.get())) {
            modelMatrix = glm::mat4(1.0f);
            ShaderManager::defaultShader->setMat4("model", sphere->modelMatrix);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, sphere->textureID);
            sphere->DrawSphere();
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}

void EntityNodes::RenderTriangle(const glm::mat4& view, const glm::mat4& projection,
    const std::vector<std::unique_ptr<BaseModel>>& models)
{
    ShaderManager::defaultGridShader->Use();
    ShaderManager::defaultGridShader->setMat4("projection", projection);
    ShaderManager::defaultGridShader->setMat4("view", view);

    for (const auto& model : models) {
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        ShaderManager::defaultGridShader->setMat4("model", modelMatrix);

        if (auto* triangle = dynamic_cast<TriangleModel*>(model.get())) {
            triangle->DrawTriangle();
        }
    }
}

void EntityNodes::RenderPlane(const glm::mat4& view, const glm::mat4& projection,
     std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Planeobjidx)
{
   
    stbi_set_flip_vertically_on_load(true);

    if (ShouldAddPlane) {
        Planeobjidx = ObjectVector.size();

        std::unique_ptr<PlaneModel> newPlane = std::make_unique<PlaneModel>(currentIndex, "DefaultPlane", Planeobjidx);
        switch (Planeobjidx) {
        case 0:
            newPlane->position = glm::vec3(0.0f, 0.0f, 0.0f);
            newPlane->scale = glm::vec3(1.0f, 1.0f, 1.0f);
            break;
        case 1:
            newPlane->position = glm::vec3(1.5f, 0.0f, 0.0f);
            newPlane->scale = glm::vec3(1.0f, 1.0f, 1.0f);
            break;

        case 2:
            newPlane->position = glm::vec3(2.0f, 0.0f, 0.0f);
            newPlane->scale = glm::vec3(1.0f, 1.0f, 1.0f);
            break;
        default:
            newPlane->position = glm::vec3(posx, 2.0f, 0.0f);
            newPlane->scale = glm::vec3(1.0f, 1.0f, 1.0f);
            posx += 1.5;
            break;
        }

        newPlane->modelMatrix = glm::translate(glm::mat4(1.0f), newPlane->position);
        newPlane->modelMatrix = glm::scale(newPlane->modelMatrix, newPlane->scale);
        
        newPlane->textureID = loadTexture("Textures/default_1.jpg");

        ObjectVector.push_back(std::move(newPlane));
        
        
        ShouldAddPlane = false; // Reset the flag after adding the plane
    }
    // ##########
    if (ShouldUpdatePlane) { // then we update the cube position and scale

        int selectedIndex = SelectedDataManager::Instance().GetSelectedData()->objectIndex;

        if (selectedIndex >= 0 && selectedIndex < ObjectVector.size()) { 

            glm::vec3 newPlanePosition = glm::vec3(object_Pos[0], object_Pos[1], object_Pos[2]); // New position
            ObjectVector[selectedIndex]->position = newPlanePosition;

            glm::vec3 newPlaneScale = glm::vec3(object_Scale[0], object_Scale[1], object_Scale[2]); // New scale
            ObjectVector[selectedIndex]->scale = newPlaneScale;

            ObjectVector[selectedIndex]->modelMatrix = glm::mat4(1.0f);
            ObjectVector[selectedIndex]->modelMatrix = glm::translate(ObjectVector[selectedIndex]->modelMatrix, newPlanePosition);
            ObjectVector[selectedIndex]->modelMatrix = glm::scale(ObjectVector[selectedIndex]->modelMatrix, newPlaneScale);          

        }

        ShouldUpdatePlane = false;
    }

    // ###########


    for (const auto& model : ObjectVector) {

        ShaderManager::defaultShader->Use();
        ShaderManager::defaultShader->setMat4("projection", projection);
        ShaderManager::defaultShader->setMat4("view", view);

        if (auto* plane = dynamic_cast<PlaneModel*>(model.get())) {
            //
            modelMatrix = glm::mat4(1.0f);
            ShaderManager::defaultShader->setMat4("model", plane->modelMatrix);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, plane->textureID);
            //
            plane->DrawPlane();
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}

void EntityNodes::RenderPyramid(const glm::mat4& view, const glm::mat4& projection,
    std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Pyramidobjidx)
{
    stbi_set_flip_vertically_on_load(true);

    if (ShouldAddPyramid) {
        Pyramidobjidx = ObjectVector.size();

        std::unique_ptr<PyramidModel> newPyramid = std::make_unique<PyramidModel>(currentIndex, "DefaultPyramid", Pyramidobjidx);
       
            
        float angle = glm::radians(-90.0f); // Rotate -90 degrees on the x
       
        switch (Pyramidobjidx) {
        case 0:
            newPyramid->position = glm::vec3(0.0f, 0.0f, 0.0f);
            newPyramid->scale = glm::vec3(1.0f, 1.0f, 1.0f);
           // newPyramid->modelMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));

            break;
        case 1:
            newPyramid->position = glm::vec3(1.0f, 0.0f, 0.0f);
            newPyramid->scale = glm::vec3(1.0f, 1.0f, 1.0f);
           // newPyramid->modelMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
            break;

        case 2:
            newPyramid->position = glm::vec3(2.0f, 0.0f, 0.0f);
            newPyramid->scale = glm::vec3(1.0f, 1.0f, 1.0f);
            //newPyramid->modelMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
            break;
        default:
            newPyramid->position = glm::vec3(posx, 2.0f, 0.0f);
            newPyramid->scale = glm::vec3(1.0f, 1.0f, 1.0f);
            //newPyramid->modelMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
            posx += 1.5;
            break;
        }

        newPyramid->modelMatrix = glm::translate(glm::mat4(1.0f), newPyramid->position);
        newPyramid->modelMatrix = glm::scale(newPyramid->modelMatrix, newPyramid->scale);
       
        newPyramid->modelMatrix = glm::rotate(newPyramid->modelMatrix, angle, glm::vec3(1.0f, 0.0f, 0.0f));
        
               
        newPyramid->textureID = loadTexture("Textures/default_1.jpg");

        ObjectVector.push_back(std::move(newPyramid));

        ShouldAddPyramid = false; // Reset the flag after adding the plane
    }

    if (ShouldUpdatePyramid) { // then we update the cube position and scale

        int selectedIndex = SelectedDataManager::Instance().GetSelectedData()->objectIndex;

        if (selectedIndex >= 0 && selectedIndex < ObjectVector.size()) {

            glm::vec3 newPyramidPosition = glm::vec3(object_Pos[0], object_Pos[1], object_Pos[2]); // New position
            ObjectVector[selectedIndex]->position = newPyramidPosition;

            glm::vec3 newPyramidScale = glm::vec3(object_Scale[0], object_Scale[1], object_Scale[2]); // New scale
            ObjectVector[selectedIndex]->scale = newPyramidScale;

            glm::vec3 newPyramidRotation = glm::vec3(object_Rotation[0], object_Rotation[1], object_Rotation[2]); // New rotation angles
            ObjectVector[selectedIndex]->rotation = newPyramidRotation;

            ObjectVector[selectedIndex]->modelMatrix = glm::mat4(1.0f);
            ObjectVector[selectedIndex]->modelMatrix = glm::translate(ObjectVector[selectedIndex]->modelMatrix, newPyramidPosition);
            ObjectVector[selectedIndex]->modelMatrix = glm::scale(ObjectVector[selectedIndex]->modelMatrix, newPyramidScale);

            ObjectVector[selectedIndex]->modelMatrix = glm::rotate(ObjectVector[selectedIndex]->modelMatrix,
                glm::radians(newPyramidRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            ObjectVector[selectedIndex]->modelMatrix = glm::rotate(ObjectVector[selectedIndex]->modelMatrix,
                glm::radians(newPyramidRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            ObjectVector[selectedIndex]->modelMatrix = glm::rotate(ObjectVector[selectedIndex]->modelMatrix,
                glm::radians(newPyramidRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        }

        ShouldUpdatePyramid = false;
    }
   
   
    
    for (const auto& model : ObjectVector) {
        ShaderManager::defaultShader->Use();
        ShaderManager::defaultShader->setMat4("projection", projection);
        ShaderManager::defaultShader->setMat4("view", view);
        
       
        if (auto* pyramid = dynamic_cast<PyramidModel*>(model.get())) {

        modelMatrix = glm::mat4(1.0f);
        ShaderManager::defaultShader->setMat4("model", pyramid->modelMatrix);
       
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, pyramid->textureID);

            pyramid->DrawPyramid();
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        
    }
}
// #######################################################################################################################
// ###########################################    LIGHTING SECTION         ###############################################
// #######################################################################################################################

void EntityNodes::RenderSunLightSprite(const glm::mat4& view, const glm::mat4& projection,
    std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& LightIdx)
{
    stbi_set_flip_vertically_on_load(true);

    if (ShouldAddSunLight) {
        LightIdx = ObjectVector.size();

        if (MAX_SUN_LIGHTS < 1) {
            std::unique_ptr<LightSprite> newLight = std::make_unique<LightSprite>(currentIndex, "Default_Sun", LightIdx);

            newLight->position = glm::vec3(0.0f, 1.0f, 0.0f);
            newLight->scale = glm::vec3(1.0f, 1.0f, 1.0f);

            newLight->modelMatrix = glm::translate(glm::mat4(1.0f), newLight->position);
            newLight->modelMatrix = glm::scale(newLight->modelMatrix, newLight->scale);

            newLight->textureID = loadTexture("Textures/lighting/sun_01.png");
            //newLight->textureID = loadTexture("Textures/lighting/sun_01.png");

            ObjectVector.push_back(std::move(newLight));
        }
        MAX_SUN_LIGHTS++;
        ShouldAddSunLight = false; // Reset the flag after adding the plane
    }
    // ##########
    if (ShouldUpdateLight) { // then we update the cube position and scale

        int selectedIndex = SelectedDataManager::Instance().GetSelectedData()->objectIndex;

        if (selectedIndex >= 0 && selectedIndex < ObjectVector.size()) {

            glm::vec3 newLightPosition = glm::vec3(object_Pos[0], object_Pos[1], object_Pos[2]); // New position
            ObjectVector[selectedIndex]->position = newLightPosition;

            glm::vec3 newLightScale = glm::vec3(object_Scale[0], object_Scale[1], object_Scale[2]); // New scale
            ObjectVector[selectedIndex]->scale = newLightScale;

            ObjectVector[selectedIndex]->modelMatrix = glm::mat4(1.0f);
            ObjectVector[selectedIndex]->modelMatrix = glm::translate(ObjectVector[selectedIndex]->modelMatrix, newLightPosition);
            ObjectVector[selectedIndex]->modelMatrix = glm::scale(ObjectVector[selectedIndex]->modelMatrix, newLightScale);

        }

        ShouldUpdateLight = false;
    }

    for (const auto& model : ObjectVector) {
      
        ShaderManager::LightShader->Use();
        ShaderManager::LightShader->setMat4("view", view);
        ShaderManager::LightShader->setMat4("projection", projection);
       
        if (auto* light = dynamic_cast<LightSprite*>(model.get())) {

            ShaderManager::LightShader->setVec3("lightPos", light->position);
            ShaderManager::LightShader->setFloat("scale", 0.3f);
            //
            modelMatrix = glm::mat4(1.0f);
            ShaderManager::LightShader->setMat4("model", light->modelMatrix);
           
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, light->textureID);
            //
            light->DrawLight();
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

}

void EntityNodes::RenderPointLightSprite(const glm::mat4& view, const glm::mat4& projection,
    std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& LightIdx)
{
    stbi_set_flip_vertically_on_load(true);

    if (ShouldAddPointLight) {
        LightIdx = ObjectVector.size();

        if (MAX_POINT_LIGHTS >= 10) {
            std::unique_ptr<LightSprite> newPointLight = std::make_unique<LightSprite>(currentIndex, "Default_Point", LightIdx);

            newPointLight->position = glm::vec3(0.5f, 1.0f, 0.0f);
            newPointLight->scale = glm::vec3(1.0f, 1.0f, 1.0f);

            newPointLight->modelMatrix = glm::translate(glm::mat4(1.0f), newPointLight->position);
            newPointLight->modelMatrix = glm::scale(newPointLight->modelMatrix, newPointLight->scale);

            newPointLight->textureID = loadTexture("Textures/lighting/light_01.png");
           
            ObjectVector.push_back(std::move(newPointLight));
        }
        MAX_POINT_LIGHTS++;
        ShouldAddPointLight = false; // Reset the flag after adding the plane
    }
    // ##########
    if (ShouldUpdateLight) { // then we update the cube position and scale

        int selectedIndex = SelectedDataManager::Instance().GetSelectedData()->objectIndex;

        if (selectedIndex >= 0 && selectedIndex < ObjectVector.size()) {

            glm::vec3 newPointLightPosition = glm::vec3(object_Pos[0], object_Pos[1], object_Pos[2]); // New position
            ObjectVector[selectedIndex]->position = newPointLightPosition;

            glm::vec3 newPointLightScale = glm::vec3(object_Scale[0], object_Scale[1], object_Scale[2]); // New scale
            ObjectVector[selectedIndex]->scale = newPointLightScale;

            ObjectVector[selectedIndex]->modelMatrix = glm::mat4(1.0f);
            ObjectVector[selectedIndex]->modelMatrix = glm::translate(ObjectVector[selectedIndex]->modelMatrix, newPointLightPosition);
            ObjectVector[selectedIndex]->modelMatrix = glm::scale(ObjectVector[selectedIndex]->modelMatrix, newPointLightScale);

        }

        ShouldUpdateLight = false;
    }

    for (const auto& model : ObjectVector) {

        ShaderManager::LightShader->Use();
        ShaderManager::LightShader->setMat4("view", view);
        ShaderManager::LightShader->setMat4("projection", projection);

        if (auto* pointlight = dynamic_cast<LightSprite*>(model.get())) {

            ShaderManager::LightShader->setVec3("lightPos", pointlight->position);
            ShaderManager::LightShader->setFloat("scale", 0.3f);
            //
            modelMatrix = glm::mat4(1.0f);
            ShaderManager::LightShader->setMat4("model", pointlight->modelMatrix);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, pointlight->textureID);
            //
            pointlight->DrawLight();
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}

void EntityNodes::RenderSpotLightSprite(const glm::mat4& view, const glm::mat4& projection, std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Lightidx)
{
}

void EntityNodes::RenderAreaLightSprite(const glm::mat4& view, const glm::mat4& projection, std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Lightidx)
{
}
// ###########################################    END LIGHTING SECTION      ###############################################

// ###########################################    TERRAIN SECTION           ###############################################
void EntityNodes::RenderTerrainFloor(const glm::mat4& view, const glm::mat4& projection,
    std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Terrainidx, Camera camera)
{
    stbi_set_flip_vertically_on_load(true);

    if (ShouldAddFloor) {
        TerrainIdx = ObjectVector.size();

        std::unique_ptr<FloorModel> newTerrain = std::make_unique<FloorModel>(currentIndex, "DefaultFloor", TerrainIdx);
        newTerrain->position = glm::vec3(0.0f, -0.5f, 0.0f);
        newTerrain->scale = glm::vec3(10.0f, 10.0f, 10.0f);

       
        newTerrain->modelMatrix = glm::translate(glm::mat4(1.0f), newTerrain->position);
        newTerrain->modelMatrix = glm::scale(newTerrain->modelMatrix, newTerrain->scale);

        newTerrain->textureID = loadTexture("Textures/Terrain/black-limestone_s.jpg");
        //newLight->textureID = loadTexture("Textures/lighting/sun_01.png");

        ObjectVector.push_back(std::move(newTerrain));

        ShouldAddFloor = false; // Reset the flag after adding the plane
    }
    // ##########
    if (ShouldUpdateFloor) { // then we update the cube position and scale

        int selectedIndex = SelectedDataManager::Instance().GetSelectedData()->objectIndex;

        if (selectedIndex >= 0 && selectedIndex < ObjectVector.size()) {

            glm::vec3 newTerrainPosition = glm::vec3(object_Pos[0], object_Pos[1], object_Pos[2]); // New position
            ObjectVector[selectedIndex]->position = newTerrainPosition;

            glm::vec3 newTerrainScale = glm::vec3(object_Scale[0], object_Scale[1], object_Scale[2]); // New scale
            ObjectVector[selectedIndex]->scale = newTerrainScale;

            ObjectVector[selectedIndex]->modelMatrix = glm::mat4(1.0f);
            ObjectVector[selectedIndex]->modelMatrix = glm::translate(ObjectVector[selectedIndex]->modelMatrix, newTerrainPosition);
            ObjectVector[selectedIndex]->modelMatrix = glm::scale(ObjectVector[selectedIndex]->modelMatrix, newTerrainScale);

        }

        ShouldUpdateFloor = false;
    }
       
    glm::vec3 sunDirection(0.0f, -1.0f, 0.0f); // default fallback
    glm::vec3 pointLightDirection(2.0f, 3.0f, 1.0f);
    // Loop through objects to find your LightSprite:
    for (const auto& model : ObjectVector) {
        if (auto* light = dynamic_cast<LightSprite*>(model.get())) {
            sunDirection = glm::normalize(glm::vec3(0.0f) - light->position);
            break; // Assuming one sun light
        }
    }
    // pointlight
    for (const auto& model : ObjectVector) {
        if (auto* pointlight = dynamic_cast<LightSprite*>(model.get())) {
            pointLightDirection = pointlight->position;
            break; 
        }
    }



    ShaderManager::TerrainShader->Use();
    ShaderManager::TerrainShader->setMat4("view", view);
    ShaderManager::TerrainShader->setMat4("projection", projection);

    ShaderManager::TerrainShader->setVec3("SunLight.direction", sunDirection);
    ShaderManager::TerrainShader->setVec3("SunLight.color", glm::vec3(1.0f, 0.0f, 0.0f));
    ShaderManager::TerrainShader->setFloat("SunLight.intensity", 0.3f);

    ShaderManager::TerrainShader->setVec3("PointLights[0].position", pointLightDirection);
   // ShaderManager::TerrainShader->setVec3("PointLights[0].position", glm::vec3(2.0f, 3.0f, 1.0f));
    ShaderManager::TerrainShader->setVec3("PointLights[0].color", glm::vec3(0.0f, 0.0f, 0.6f));
    ShaderManager::TerrainShader->setFloat("PointLights[0].intensity", 1.0f);
    ShaderManager::TerrainShader->setFloat("PointLights[0].radius", 10.0f); // affects attenuation

    for (const auto& model : ObjectVector) {

        
     
        if (auto* terrain = dynamic_cast<FloorModel*>(model.get())) {

           

            // 5. Pass the direction to the shader
            //ShaderManager::TerrainShader->setVec3("SunLight.direction", sunDirection);
            ShaderManager::TerrainShader->setMat4("model", terrain->modelMatrix);
                               
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, terrain->textureID);
            
            terrain->DrawFloor();
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

}

// ###########################################    END TERRAIN SECTION       ###############################################

void EntityNodes::DrawSelectionBox(std::vector<std::unique_ptr<BaseModel>>& ObjectVector)
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    for (const auto& obj : ObjectVector) {
        if (obj->isSelected) {
            ImVec2 min = ImVec2(obj->position.x - obj->scale.x / 2.0f, obj->position.y - obj->scale.y / 2.0f);
            ImVec2 max = ImVec2(obj->position.x + obj->scale.x / 2.0f, obj->position.y + obj->scale.y / 2.0f);

            // Draw a red box around the object
            drawList->AddRect(min, max, IM_COL32(255, 0, 0, 255));
        }
    }

    
}





/* ShaderManager::TerrainShader->setVec3("light.ambient", 0.3f, 0.3f, 0.3f);
    ShaderManager::TerrainShader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    ShaderManager::TerrainShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    ShaderManager::TerrainShader->setVec3("matirial.specular", 0.5f, 0.5f, 0.5f);
    ShaderManager::TerrainShader->setFloat("matirial.shininess", 30.0f);*/

    //ShaderManager::TerrainShader->setVec3("viewPos", camera.Position);


/*ShaderManager::TerrainShader->Use();
        ShaderManager::TerrainShader->setMat4("view", view);
        ShaderManager::TerrainShader->setMat4("projection", projection);
        ShaderManager::defaultShader->setVec3("lightColor", lightColor);
        ShaderManager::defaultShader->setVec3("lightPos", lightPos);
        ShaderManager::TerrainShader->setVec3("light.position", lightPos);
        ShaderManager::TerrainShader->setVec3("light.color", lightColor);
        ShaderManager::defaultShader->setFloat("ambientFactor", ambientFactor);*/



        /*ShaderManager::TerrainShader->setVec3("terrainPos", terrain->position);
                   ShaderManager::TerrainShader->setFloat("scale", 25.0f);*/

                   /* model = glm::translate(glm::mat4(1.0), modelPos) * glm::scale(glm::mat4(1.0), modelScale[i]);

                              ShaderManager::TerrainShader->setMet4("model", model);*/


                              // ShaderManager::TerrainShader->setInt("texture_diffuse", 0);