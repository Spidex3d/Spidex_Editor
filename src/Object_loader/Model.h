#pragma once
#include <vector>
#include <string>
#include "../Ecs/BaseModel.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Mesh.h"
#include "Texture.h"

class Model : public BaseModel
{
public:
	Model(int inx, const std::string& name, int glTFModelIndex);

	void LoadModel(const std::string& fileName);
	//void ProcessMesh(aiMesh* mesh, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

	void RenderModel();
	void ClearModel();

	~Model();

private:

	int idx;
	const std::string& name;
	int glTFModelIndex;

	void LoadNode(aiNode* node, const aiScene* scene);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterials(const aiScene* scene);

	std::vector<Mesh*> meshList;
	std::vector<Texture*> textureList;
	std::vector<unsigned int> meshToTex;
};



