#pragma once
#include "../Headers/Config.h"
#include <vector>
#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "../Shader/ShaderManager.h"
#include "../Camera/Camera.h"

#include <stb\stb_image.h>

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

	void EntityProperties(); // not in use yet !!
	
	// Render the editor Grid
	void RenderGrid(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Gridobjidx);

	void RenderScene(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, Shader& shader, Camera& camera);
	//// gltf Model files
	void RenderModelFiles(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& glTFModelIndex, Shader& shader, Camera& camera);

	// Obj Model files
	void RenderObjFiles(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& ModleObjidx);

	// Cube
	void RenderCube(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Cubeobjidx);
		// Triangel
	void RenderTriangle(const glm::mat4& view, const glm::mat4& projection, const std::vector<std::unique_ptr<BaseModel>>& models);
	// Plane
	void RenderPlane(const glm::mat4& view, const glm::mat4& projection,
		 std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Planeobjidx);
	// Pyramid
	void RenderPyramid(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Planeobjidx);

	

	void DrawSelectionBox(std::vector<std::unique_ptr<BaseModel>>& ObjectVector);
	


	
private:
	// Object Mesh Editor
	
	bool showObjectEditor = false;
	char nameBuffer[128] = "";
	
	glm::mat4 modelMatrix;

	int objectIndex; // needed
	
	void onRightClick(int objectId) {
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			ImGui::OpenPopup(("NodePopup" + std::to_string(objectId)).c_str());
		}
	
	}
	
	float cam_pos_val[3] = { 0.0f, 0.0f, 0.0f };
	float cam_rot_val[3] = { 0.0f, 0.0f, 0.0f }; 
	float cam_scale_val[3] = { 0.0f, 0.0f, 0.0f }; 
	
};

