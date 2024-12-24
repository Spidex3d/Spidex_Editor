#pragma once

// This manages the selected data for the imguitreeveiw
class SelectedDataManager {
public:
    static SelectedDataManager& Instance() {
        static SelectedDataManager instance;
        return instance;
    }

    BaseModel* GetSelectedData() const {
        return selectedData;
    }

    void SetSelectedData(BaseModel* data) {
        selectedData = data;
    }

private:
    SelectedDataManager() : selectedData(nullptr) {}
    ~SelectedDataManager() {}

    SelectedDataManager(const SelectedDataManager&) = delete;
    SelectedDataManager& operator=(const SelectedDataManager&) = delete;

    BaseModel* selectedData;
};

/*
#include "EntityNode.h"
#include "SelectedDataManager.h"

void EntityNode::EntityManagmentSystem(const std::vector<std::unique_ptr<BaseModel>>& MainVector, int& currentIndex, int& indexCube,
    int& indexPlane, int& indexSphere, int& indexLight, int& indexTypeID) {
    ImGui::Begin("Entity Management System");
    ImGui::SeparatorText("Scene Collection");

    auto flags = ImGuiTreeNodeFlags_DefaultOpen;
    if (ImGui::TreeNodeEx("Editor Scene", flags)) {
        for (const auto& data : MainVector) {
            ImGuiTreeNodeFlags nodeFlags = flags | (SelectedDataManager::Instance().GetSelectedData() == data.get() ? ImGuiTreeNodeFlags_Selected : 0);
           ImGuiTreeNodeFlags nodeFlags = flags | (selectedData == data.get() ? ImGuiTreeNodeFlags_Selected : 0);
           bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)data->index, nodeFlags,
                "Object: %d : %s %d : Type ID : %d",
                data->index, data->objectName.c_str(), data->objectIndex, data->objectTypeID);

            if (ImGui::IsItemClicked()) {
                SelectedDataManager::Instance().SetSelectedData(data.get());
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


*/
