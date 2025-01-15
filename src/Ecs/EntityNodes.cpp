#include "EntityNodes.h"




EntityNodes* EntityNodes::Instance() {
    static EntityNodes component;
    return &component;
}

void EntityNodes::Initialize()
{

    BaseModel* selectedData = nullptr;  // Define the external variable

    // this will change in the future
    ObjectVector.push_back(std::make_unique<CubeModel>(currentIndex++, "DefaultCube", Cubeobjidx++, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 4.0f));
    ObjectVector.push_back(std::make_unique<CubeModel>(currentIndex++, "DefaultCube", Cubeobjidx++, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 4.0f));
    ObjectVector.push_back(std::make_unique<TriangleModel>(currentIndex++, "DefaultTriangel", Triangleobjidx++));
    ObjectVector.push_back(std::make_unique<TriangleModel>(currentIndex++, "DefaultTriangel", Triangleobjidx++)); 
    ObjectVector.push_back(std::make_unique<TriangleModel>(currentIndex++, "DefaultTriangel", Triangleobjidx++)); 
    ObjectVector.push_back(std::make_unique<PlaneModel>(currentIndex++, "DefaultPlane", Planeobjidx++));
    ObjectVector.push_back(std::make_unique<PlaneModel>(currentIndex++, "DefaultPlane", Planeobjidx++));
    ObjectVector.push_back(std::make_unique<PlaneModel>(currentIndex++, "DefaultPlane", Planeobjidx++));
    ObjectVector.push_back(std::make_unique<PlaneModel>(currentIndex++, "DefaultPlane", Planeobjidx++));
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
                        " %d : %s : %d : Type ID : %d",
                        //"Object: %d : %s : %d : Type ID : %d",
                        data->index, data->objectName.c_str(), data->objectIndex, data->objectTypeID);

                    if (ImGui::IsItemClicked()) {
                        SelectedDataManager::Instance().SetSelectedData(data.get());
                        std::cout << "Data Selected was " << data->objectName.c_str() << " : " << data->objectIndex << std::endl;
                    }

                    if (ImGui::IsItemHovered()) {
                        // Perform actions when the node is hovered
                        ImGui::SetTooltip("Right click to Edit %s | %d", data->objectName.c_str(), data->objectTypeID);
                        
                    }

                    if (nodeOpen) {
                        //this->onRightClick(data->objectIndex); // Pass a unique identifier
                        this->onRightClick(data->index); // Pass a unique identifier

                        if (ImGui::BeginPopup(("NodePopup" + std::to_string(data->index)).c_str())) {
                            ImGui::TextColored(COLOR_LIGHTBLUE, ICON_FA_EDIT " ENTITY");
                            ImGui::Separator(); // Draw a line
                            if (ImGui::Selectable(ICON_FA_PEN_ALT " Edit")) {
                                std::cout << "Object Selected To Edit " << data->objectName.c_str() << " : " << data->objectIndex << std::endl;
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

void EntityNodes::EntityProperties() {

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
