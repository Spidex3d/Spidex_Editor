#pragma once
#include "../Headers/Config.h"
#include "../Ecs/EntityComponents.h"
#include <vector>
#include <memory>
#include <string>

#include "../Ecs/SelectedDataManager.h"

//static BaseModel* selectedData = nullptr;

class EntityNode {
public:
  
        static EntityNode* Instance() {
            static EntityNode component;
            return &component;
        }
        // This fill the list of objects and allows us to select an object
        void EntityManagmentSystem(std::vector<std::unique_ptr<BaseModel>>&MainVector, int& currentIndex, int& index, int& objectIndex,
                int& indexTypeID) {

            ImGui::Begin("Entity Management System");
            ImGui::SeparatorText("Scene Collection");

            auto flags = ImGuiTreeNodeFlags_DefaultOpen;
            if (ImGui::TreeNodeEx("Editor Scene", flags)) {
                for (const auto& data : MainVector) {
                   // ImGuiTreeNodeFlags nodeFlags = flags | (selectedData == data.get() ? ImGuiTreeNodeFlags_Selected : 0);
                    ImGuiTreeNodeFlags nodeFlags = flags | (SelectedDataManager::Instance().GetSelectedData() == data.get() ? ImGuiTreeNodeFlags_Selected : 0);

                    bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)data->objectIndex, nodeFlags,
                        "Object: %d : %s : %d : Type ID : %d",
                       data->index, data->objectName.c_str(), data->objectIndex, data->objectTypeID);

                    /*bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)data->index, nodeFlags,
                        "Object: %d : %s %d : Type ID : %d",
                        data->index, data->objectName.c_str(), data->objectIndex, data->objectTypeID);*/

                    if (ImGui::IsItemClicked()) {
                        //selectedData = data.get();
                        SelectedDataManager::Instance().SetSelectedData(data.get());
                        std::cout << "Data Selected was " << data->objectName.c_str() << std::endl;
                    }

                    if (nodeOpen) {
                        data->RenderImGui();
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
            ImGui::End();
        }
        
        // this is where we can change the properties of an object
        void EntityProperties() {
            ImGui::Begin(ICON_FA_CUBES" Objects Properties");

            ImGui::End();
        }
};





