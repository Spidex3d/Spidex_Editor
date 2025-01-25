#pragma once
#include "../Headers/Config.h"
#include <vector>
#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "../Shader/ShaderManager.h"

#include "ObjectManager.h"
#include "SelectedDataManager.h"

#include "../Headers/GlobalVars.h"


class EntityNodes
{
public:
	
	static EntityNodes* Instance();

	void Initialize();

	void ObjectEditor(std::vector<std::unique_ptr<BaseModel>>& selectedData);

	std::vector<std::unique_ptr<BaseModel>>& GetModels();

	void EntityManagmentSystem(std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex,
		int& index, int& objectIndex, int& indexTypeID);

	void EntityProperties();
	
	//void RenderGrid(const glm::mat4& view, const glm::mat4& projection);
	void RenderGrid(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Gridobjidx);

	
	void RenderScene(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Cubeobjidx);

	//void RenderCube(const glm::mat4& view, const glm::mat4& projection, const std::vector<std::unique_ptr<BaseModel>>& models);
	void RenderCube(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Cubeobjidx);
	void RenderTriangle(const glm::mat4& view, const glm::mat4& projection, const std::vector<std::unique_ptr<BaseModel>>& models);
	void RenderPlane(const glm::mat4& view, const glm::mat4& projection, const std::vector<std::unique_ptr<BaseModel>>& models);


private:
	// Object Mesh Editor
	int objectUpdateIndex = -1; // this needs moving at some point
	bool showObjectEditor = false;
	char nameBuffer[128] = "";
	
	 //bool ShouldAddCube = false;
	glm::mat4 modelMatrix;

		
	// End Object Mesh Editor
	std::vector<std::unique_ptr<BaseModel>> ObjectVector; // the vector for all Models
	//std::vector<std::unique_ptr<BaseModel>> LightVector; // the vector for all Lights 
	//std::vector<std::unique_ptr<BaseModel>> SceneVector; // the vector for all Sky, water, 

	//int currentIndex;
	int objectIndex;
	
	void onRightClick(int objectId) {
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			ImGui::OpenPopup(("NodePopup" + std::to_string(objectId)).c_str());
		}
	
	}
	
	float cam_pos_val[3] = { 0.0f, 0.0f, 0.0f };
	float cam_rot_val[3] = { 0.0f, 0.0f, 0.0f }; 
	float cam_scale_val[3] = { 0.0f, 0.0f, 0.0f }; 
	
};

