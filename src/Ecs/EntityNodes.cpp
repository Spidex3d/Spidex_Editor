#include "EntityNodes.h"
#include "../Windows/spx_FileDialog.h"

// bool ShouldAddCube = false;
int cubeIndex = -1;

EntityNodes* EntityNodes::Instance() {
    static EntityNodes component;
    return &component;
}
// TO DO ADD THIS TO SETTINGS
float test1 = 2.0f;
int test2 = 40;

void EntityNodes::Initialize()
{
    BaseModel* selectedData = nullptr;  // Define the external variable
        
    // Add The Default Cube
   // ObjectVector.push_back(std::make_unique<MainGrid>(0, "Main Grid", 1.0f, 20));
    ObjectVector.push_back(std::make_unique<MainGrid>(0, "Main Grid", test1, test2));
    ObjectVector.push_back(std::make_unique<CubeModel>(currentIndex++, "DefaultCube", Cubeobjidx++));

    /*ObjectVector.push_back(std::make_unique<CubeModel>(currentIndex++, "DefaultCube", Cubeobjidx++));
    ObjectVector.push_back(std::make_unique<TriangleModel>(currentIndex++, "DefaultTriangel", Triangleobjidx++));
    ObjectVector.push_back(std::make_unique<TriangleModel>(currentIndex++, "DefaultTriangel", Triangleobjidx++)); 
    ObjectVector.push_back(std::make_unique<TriangleModel>(currentIndex++, "DefaultTriangel", Triangleobjidx++)); 
    ObjectVector.push_back(std::make_unique<PlaneModel>(currentIndex++, "DefaultPlane", Planeobjidx++));
    ObjectVector.push_back(std::make_unique<PlaneModel>(currentIndex++, "DefaultPlane", Planeobjidx++));
    ObjectVector.push_back(std::make_unique<PlaneModel>(currentIndex++, "DefaultPlane", Planeobjidx++));
    ObjectVector.push_back(std::make_unique<PlaneModel>(currentIndex++, "DefaultPlane", Planeobjidx++));*/
   
}

void EntityNodes::ObjectEditor(std::vector<std::unique_ptr<BaseModel>>& selectedData)
{
    if (showObjectEditor) { // if this is true open the editor window
        ImGui::Begin("Mesh Object Editor", &showObjectEditor);
        ImGui::InputText("Object Name", nameBuffer, IM_ARRAYSIZE(nameBuffer));
        ImGui::TextColored(COLOR_LIGHTBLUE, ICON_FA_EDIT " Mesh Editor");
        ImGui::SeparatorText("Object Editor");

        //##

        ImGui::BeginTable("Edit Table", 1, ImGuiTableFlags_Reorderable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders);

        ImGui::TableNextColumn();
        
        ImGui::DragFloat3("##pos", objPostion, 0.0f, 0.0f, 0.0f);
        ImGui::SameLine();
        if (ImGui::Button("Reset_Position")) {
            objPostion[0] = 0.0f, objPostion[1] = 0.0f, objPostion[2] = 0.0f;
        }

        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        ImGui::DragFloat3("##Scale", objScale, 1.0f, 1.0f, 1.0f);
        ImGui::SameLine();
        if (ImGui::Button("Reset Scale")) {
            objScale[0] = 1.0f, objScale[1] = 1.0f, objScale[2] = 1.0f;
        }

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        
        ImGui::DragFloat3("##Rot", objRotation, 0.0f, 0.0f, 0.0f);
        ImGui::SameLine();
        if (ImGui::Button("Resetr Rotation")) {
            objRotation[0] = 0.0f, objRotation[1] = 0.0f, objRotation[2] = 0.0f;
        }

        ImGui::EndTable();
        //##

        ImGui::TextColored(COLOR_LIGHTBLUE, ICON_FA_IMAGE " Texture Editor");
        ImGui::SeparatorText(" Texture Editor");
       

        spx_FileDialog openDialog;
        if (ImGui::Button("Set New Texture")) {
            //std::string myTexturePath = openFileDialog();
            std::string myTexturePath = openDialog.openFileDialog();
            if (!myTexturePath.empty()) {
                std::cout << "Texture path selected: " << myTexturePath << std::endl;
                objectUpdateIndex = SelectedDataManager::Instance().GetSelectedData()->objectIndex; // just added
                if (objectUpdateIndex != -1) {
                    std::cout << "Updating texture for cube index: " << objectUpdateIndex << std::endl;
                   // mycubes[objectUpdateIndex].textureID = loadTexture(myTexturePath);
                   // std::cout << "New texture ID: " << mycubes[objectUpdateIndex].textureID << std::endl;
                }
                else {
                    std::cout << "objectUpdateIndex is not set correctly." << std::endl;
                }
            }
            else {
                std::cout << "No texture path selected." << std::endl;
            }
        }
        
        if (ImGui::Button("Update")) { // Update the object's name 

            // Check if nameBuffer is not empty
            if (strlen(nameBuffer) > 0) {
                if (SelectedDataManager::Instance().GetSelectedData() != nullptr) {
                    SelectedDataManager::Instance().GetSelectedData()->objectName = std::string(nameBuffer);

                    shouldUpdateObject = true;
                    objectUpdateIndex = SelectedDataManager::Instance().GetSelectedData()->objectIndex; // Ensure this is set correctly
                    
                    std::cout << "Object update index set to: " << objectUpdateIndex << std::endl;
                    
                    std::cout << "Selected " << SelectedDataManager::Instance().GetSelectedData()->objectName << " Index " <<
                        SelectedDataManager::Instance().GetSelectedData()->objectIndex << std::endl;
                }
            }
                showObjectEditor = false;
        }

        ImGui::End();
    }
}

std::vector<std::unique_ptr<BaseModel>>& EntityNodes::GetModels()
{
    return ObjectVector;
}

void EntityNodes::EntityManagmentSystem(std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& index, int& objectIndex, int& indexTypeID) {
    ImGui::Begin("Entity Management System"); // start of the window

    // TEST
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
                                case 1:
                                    showObjectEditor = true;

                                    std::cout << "Data Selected  is a Cube " << data->objectName.c_str() << " : " << data->objectIndex << std::endl;
                                    LogInternals::Instance()->Debug("Data Selected  is a Cube");
                                    break;
                                case 2:
                                    break;
                                case 3:
                                    break;
                                case 4: // cube
                                   
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

// ####

void EntityNodes::RenderScene(const glm::mat4& view, const glm::mat4& projection,
         std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Cubeobjidx)
{
  
    EntityNodes::RenderCube(view, projection, ObjectVector, currentIndex, Cubeobjidx);
    EntityNodes::RenderTriangle(view, projection, ObjectVector);
    EntityNodes::RenderPlane(view, projection, ObjectVector);
}

// ####


void EntityNodes::RenderCube(const glm::mat4& view, const glm::mat4& projection,
    std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Cubeobjidx) {
    ShaderManager::defaultGridShader->Use();
    ShaderManager::defaultGridShader->setMat4("projection", projection);
    ShaderManager::defaultGridShader->setMat4("view", view);

   

    if (ShouldAddCube) {  
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f, 1.0f, 1.0f));

        // Add the cube to the vector
        ObjectVector.push_back(std::make_unique<CubeModel>(currentIndex++, "DefaultCube", Cubeobjidx++));
        ShouldAddCube = false; // Reset the flag after adding the cube
    }

    for (const auto& model : ObjectVector) {
        if (auto* cube = dynamic_cast<CubeModel*>(model.get())) {
            modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
           // modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
            ShaderManager::defaultGridShader->setMat4("model", modelMatrix);
            cube->DrawCube();
        }
    }
}




void EntityNodes::RenderTriangle(const glm::mat4& view, const glm::mat4& projection, const std::vector<std::unique_ptr<BaseModel>>& models)
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

void EntityNodes::RenderPlane(const glm::mat4& view, const glm::mat4& projection, const std::vector<std::unique_ptr<BaseModel>>& models)
{
    ShaderManager::defaultGridShader->Use();
    ShaderManager::defaultGridShader->setMat4("projection", projection);
    ShaderManager::defaultGridShader->setMat4("view", view);

    for (const auto& model : models) {
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(3.0f, 0.0f, 0.0f));

        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        ShaderManager::defaultGridShader->setMat4("model", modelMatrix);

        if (auto* plane = dynamic_cast<PlaneModel*>(model.get())) {
            plane->DrawPlane();
        }
    }
}







//#include "EntityNodes.h"
//
//EntityNodes* EntityNodes::Instance()
//{
//    static EntityNodes component;
//    return &component;
//}
//
//void EntityNodes::EntityManagmentSystem(std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& index, int& objectIndex, int& indexTypeID)
//{
//    ImGui::Begin("Entity Management System"); // start of the window
//    ImGui::SeparatorText("Scene Collection");
//
//    auto flags = ImGuiTreeNodeFlags_DefaultOpen;
//    if (ImGui::TreeNodeEx("Editor Scene", flags))
//    {
//
//        for (const auto& data : ObjectVector) {
//
//            ImGuiTreeNodeFlags nodeFlags = flags | (SelectedDataManager::Instance().GetSelectedData() == data.get() ? ImGuiTreeNodeFlags_Selected : 0);
//
//            bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)data->objectIndex, nodeFlags,
//                "Object: %d : %s : %d : Type ID : %d",
//                data->index, data->objectName.c_str(), data->objectIndex, data->objectTypeID);
//                        
//            if (ImGui::IsItemClicked()) {
//                SelectedDataManager::Instance().SetSelectedData(data.get());
//                std::cout << "Data Selected was " << data->objectName.c_str() << std::endl;
//            }
//
//            if (ImGui::IsItemHovered()) {
//                 // Perform actions when the node is hovered
//                ImGui::SetTooltip("Right click to Edit %s | %d", data->objectName.c_str(), data->objectTypeID);
//                //ImGui::SetTooltip("Right click to Edit %d", data->objectTypeID);
//            }
//
//            if (nodeOpen) {
//                this->onRightClick(data->objectIndex); // Pass a unique identifier
//                //this->onRightClick();
//
//
//                if (ImGui::BeginPopup(("NodePopup" + std::to_string(data->objectIndex)).c_str())) {
//                    ImGui::TextColored(COLOR_LIGHTBLUE, ICON_FA_EDIT " ENTITY");
//                    ImGui::Separator(); // Draw a line
//
//                    ImGui::EndPopup();
//
//                }
//
//
//
//                //if (ImGui::BeginPopup("NodePopup")) {
//                //    ImGui::TextColored(COLOR_LIGHTBLUE, ICON_FA_EDIT " ENTITY");
//                //    ImGui::Separator(); // Draw a line
//
//                //    ImGui::EndPopup();   
//                //}              
//
//                ImGui::TreePop();
//            }
//        }
//        ImGui::TreePop();
//    }
//
//    ImGui::End();
//}
//
//void EntityNodes::EntityProperties()
//{
//
//}


/*
#include "EntityNodes.h"

EntityNodes* EntityNodes::Instance() {
    static EntityNodes component;
    return &component;
}

void EntityNodes::EntityManagmentSystem(std::vector<std::unique_ptr<BaseModel>>& ObjectVector,
                int& currentIndex, int& index, int& objectIndex, int& indexTypeID) {

    ImGui::Begin("Entity Management System"); // start of the window
    ImGui::SeparatorText("Scene Collection");

    auto flags = ImGuiTreeNodeFlags_DefaultOpen;
         if (ImGui::TreeNodeEx("Editor Scene", flags)) {

        for (const auto& data : ObjectVector) {

            ImGuiTreeNodeFlags nodeFlags = flags | (SelectedDataManager::Instance().GetSelectedData() == data.get() ? ImGuiTreeNodeFlags_Selected : 0);

            bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)data->objectIndex, nodeFlags,
                "Object: %d : %s : %d : Type ID : %d",
                data->index, data->objectName.c_str(), data->objectIndex, data->objectTypeID);

            if (ImGui::IsItemClicked()) {
                SelectedDataManager::Instance().SetSelectedData(data.get());
                std::cout << "Data Selected was " << data->objectName.c_str() << std::endl;
            }

            if (ImGui::IsItemHovered()) {
                // Perform actions when the node is hovered
                ImGui::SetTooltip("Right click to Edit %s | %d", data->objectName.c_str(), data->objectTypeID);
                //ImGui::SetTooltip("Right click to Edit %d", data->objectTypeID);
            }

            if (nodeOpen) {
                this->onRightClick(data->objectIndex); // Pass a unique identifier

                if (ImGui::BeginPopup(("NodePopup" + std::to_string(data->objectIndex)).c_str())) {
                    ImGui::TextColored(COLOR_LIGHTBLUE, ICON_FA_EDIT " ENTITY");
                    ImGui::Separator(); // Draw a line

                    ImGui::EndPopup();
                }

                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }

    ImGui::End();
}

void EntityNodes::EntityProperties() {

}

*/
