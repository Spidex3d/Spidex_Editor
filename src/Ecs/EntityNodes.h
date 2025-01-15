#pragma once
#include "../Headers/Config.h"
#include <vector>
#include <memory>
#include <string>

#include "ObjectManager.h"
#include "SelectedDataManager.h"


class EntityNodes
{
public:

	static EntityNodes* Instance();

	void Initialize();

	std::vector<std::unique_ptr<BaseModel>>& GetModels();

	void EntityManagmentSystem(std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex,
		int& index, int& objectIndex, int& indexTypeID);

	void EntityProperties();

private:
	std::vector<std::unique_ptr<BaseModel>> ObjectVector; // the vector for all Models
	//std::vector<std::unique_ptr<BaseModel>> LightVector; // the vector for all Lights 
	//std::vector<std::unique_ptr<BaseModel>> SceneVector; // the vector for all Sky, water, 

	int currentIndex;
	int objectIndex;
	// indevidual object index's
	int Cubeobjidx;
	int Sphereobjidx;
	int Triangleobjidx;
	int Planeobjidx;


	


	void onRightClick(int objectId) {
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			ImGui::OpenPopup(("NodePopup" + std::to_string(objectId)).c_str());
		}
	}
	
	float cam_pos_val[3] = { 0.0f, 0.0f, 0.0f };
	float cam_rot_val[3] = { 0.0f, 0.0f, 0.0f }; 
	float cam_scale_val[3] = { 0.0f, 0.0f, 0.0f }; 
	


	/*void onRightClick() {
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			ImGui::OpenPopup("NodePopup");
		}
	}*/
};

