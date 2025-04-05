#include "EntityNodes.h"
#include "../Windows/spx_FileDialog.h"
#include "../Object_loader\objLoader.h"

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
                                    LogInternals::Instance()->Debug("Data Selected  is a obj file");
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
    std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex)
{
    ShaderManager::defaultShader->Use();
    ShaderManager::defaultShader->setMat4("projection", projection);
    ShaderManager::defaultShader->setMat4("view", view);
    ShaderManager::defaultShader->setMat4("model", modelMatrix);

    EntityNodes::RenderCube(view, projection, ObjectVector, currentIndex, Cubeobjidx);
    EntityNodes::RenderTriangle(view, projection, ObjectVector);
    EntityNodes::RenderPlane(view, projection, ObjectVector, currentIndex, Planeobjidx);
    EntityNodes::RenderPyramid(view, projection, ObjectVector, currentIndex, Pyramidobjidx);
    EntityNodes::RenderObjFiles(view, projection, ObjectVector, currentIndex, ModleObjidx);
   
}
void EntityNodes::RenderObjFiles(const glm::mat4& view, const glm::mat4& projection,
    std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& ModleObjidx)
{
         
    if (ShouldAddObjModel) {     
    ModleObjidx = ObjectVector.size();
    //auto newMesh = std::make_unique<objLoader>(currentIndex++, "New ObjFile", ModleObjidx);
    std::unique_ptr<objLoader> newMesh = std::make_unique<objLoader>(currentIndex++, "New ObjFile", ModleObjidx);

       
        if (newMesh->Loadobj("Assets/Models/woodcrate.obj")) {
            newMesh->objModels();          
        }
        
        switch (ModleObjidx) {
        case 1:
            newMesh->position = glm::vec3(0.0f, 0.0f, 0.0f);
            newMesh->scale = glm::vec3(1.0f, 1.0f, 1.0f);
            break;

        default:
            newMesh->position = glm::vec3(1.0f, 0.0f, 0.0f);
            newMesh->scale = glm::vec3(0.5f, 0.5f, 0.5f);
            break;
        }

        newMesh->modelMatrix = glm::mat4(1.0f);
        newMesh->modelMatrix = glm::translate(newMesh->modelMatrix, newMesh->position);
        newMesh->modelMatrix = glm::scale(newMesh->modelMatrix, newMesh->scale);

        ObjectVector.push_back(std::move(newMesh));
      
        ShouldAddObjModel = false; // Reset the flag after adding the Obj Model
    }

    if (ShouldUpdateObjModel) { // Edit and move obj model
        int selectedIndex = SelectedDataManager::Instance().GetSelectedData()->objectIndex;

        if (selectedIndex >= 0 && selectedIndex < ObjectVector.size()) {
            glm::vec3 newMeshPosition = glm::vec3(object_Pos[0], object_Pos[1], object_Pos[2]);
            ObjectVector[selectedIndex]->position = newMeshPosition;

            glm::vec3 newMeshScale = glm::vec3(object_Scale[0], object_Scale[1], object_Scale[2]);
            ObjectVector[selectedIndex]->scale = newMeshScale;

            ObjectVector[selectedIndex]->modelMatrix = glm::mat4(1.0f);
            ObjectVector[selectedIndex]->modelMatrix = glm::translate(ObjectVector[selectedIndex]->modelMatrix, newMeshPosition);
            ObjectVector[selectedIndex]->modelMatrix = glm::scale(ObjectVector[selectedIndex]->modelMatrix, newMeshScale);
        }

        ShouldUpdateObjModel = false; // Reset the flag after Editing the Obj Model
    }

    for (const auto& model : ObjectVector) {
        ShaderManager::defaultShader->Use();
        ShaderManager::defaultShader->setMat4("projection", projection);
        ShaderManager::defaultShader->setMat4("view", view);

        //if (auto* objModel = dynamic_cast<objectModel*>(model.get())) {
        if (auto* objModel = dynamic_cast<objLoader*>(model.get())) {
           // std::cout << "Dynamic cast succeeded for model at address: " << model.get() << std::endl;

            ShaderManager::defaultShader->setMat4("model", objModel->modelMatrix);
           
            objModel->objDrawModels();
        }
        else {
           // std::cout << "Dynamic cast failed for model at address: " << model.get() << std::endl;
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

        newCube->modelMatrix = glm::mat4(1.0f);
        newCube->modelMatrix = glm::translate(newCube->modelMatrix, newCube->position);
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

        //std::unique_ptr<PlaneModel> newPlane = std::make_unique<PlaneModel>(currentIndex++, "DefaultPlane", Planeobjidx++);
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

        newPlane->modelMatrix = glm::mat4(1.0f);
        newPlane->modelMatrix = glm::translate(newPlane->modelMatrix, newPlane->position);
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

        newPyramid->modelMatrix = glm::mat4(1.0f);
        newPyramid->modelMatrix = glm::translate(newPyramid->modelMatrix, newPyramid->position);
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












