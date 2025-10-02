#pragma once

#include "../Headers/Config.h"
#include <vector>
#include <memory>
#include <string>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "../Shader/ShaderManager.h"
#include "../Camera/Camera.h"

#include <stb\stb_image.h>

#include "ObjectManager.h"
#include "Player.h"
#include "../Lighting/Lighting.h"
#include "../Terrain/Terrain.h"
#include "../Sky/Sky.h"
#include "../Object_Editor/objectEdit.h"
#include "SelectedDataManager.h"

#include "../Headers/GlobalVars.h"

class EntityNodes
{
public:
	
	static EntityNodes* Instance();

	void Initialize();

	void ObjectEditor(std::vector<std::unique_ptr<BaseModel>>& selectedData);
	void LightingEditor(std::vector<std::unique_ptr<BaseModel>>& selectedData);

	 float expSmooth(float hz, float dt) { return 1.0f - std::exp(-hz * dt); }
	 void CameraFollowPlayer(Camera& cam, const PlayerModel& player, float dt, const glm::vec3& localOffset,   // Right, Up, Forward  (use z = -6 for "behind")
		float posHz = 6.0f, float rotHz = 8.0f);           // 0 => hard lock, higher => snappier
		

	 void AttachPlayerToCamera(const Camera& cam, std::vector<std::unique_ptr<BaseModel>>& objs, int playerIndex,
		const glm::vec3& localOffset);  // e.g., {0, -0.2f, 6.0f}


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
	void RendergltfFiles(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& glTFModelIndex, Shader& shader, Camera& camera);

	// Obj Model files
	void RenderObjFiles(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& ModleObjidx);
	// load a mesh for editing
	void RenderEditMeshFiles(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& HalfEdgeIdx);
	

	/*void RenderMainPlayer(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, Shader& shader, Camera& camera);

	void RenderPlayer(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& PlayerIdx);*/

	// Cube
	void RenderCube(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Cubeobjidx);

	// Sphere
	void RenderSphere(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Sphereobjidx);
	// Triangel
	void RenderTriangle(const glm::mat4& view, const glm::mat4& projection, const std::vector<std::unique_ptr<BaseModel>>& models);
	// Plane
	void RenderPlane(const glm::mat4& view, const glm::mat4& projection,
		 std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Planeobjidx);
	// Pyramid
	void RenderPyramid(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Planeobjidx);

	// ############################   LIGHTING #######################################
	void RenderSunLightSprite(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Lightidx);
	void RenderPointLightSprite(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Lightidx);
	void RenderSpotLightSprite(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Lightidx);
	void RenderAreaLightSprite(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& Lightidx);
	// ############################   END LIGHTING #######################################
	
	// ############################    TERRAIN    #######################################
	void RenderTerrain(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& TerrainIdx, Camera camera);
	
	void RenderTerrainFloor(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& TerrainIdx, Camera camera);
	// ############################   END TERRAIN #######################################
	
	// ############################   SKY BOX     #######################################
	void RenderSkyBox(const glm::mat4& view, const glm::mat4& projection,
		std::vector<std::unique_ptr<BaseModel>>& ObjectVector, int& currentIndex, int& SkyIdx, Camera camera);

	// ############################   END SKY BOX #######################################

	void DrawSelectionBox(std::vector<std::unique_ptr<BaseModel>>& ObjectVector);

	
private:
	// Object Mesh Editor
	bool showObjectEditor = false;
	//char nameBuffer[128] = "";
	
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

