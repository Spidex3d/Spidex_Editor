#include "Model.h"
#include "..\Headers\GlobalVars.h"
#include "../Shader/ShaderManager.h"

Model::Model(int idx, const std::string& name, int glTFModelIndex)
	:idx(idx), name(name), glTFModelIndex(glTFModelIndex)
{
	index = idx;
	objectName = name;
	objectIndex = glTFModelIndex;
	objectTypeID = GLTF_MODEL;
}

void Model::RenderModel()
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		unsigned int materialIndex = meshToTex[i];

		if (materialIndex < textureList.size() && textureList[materialIndex])
		{
			textureList[materialIndex]->UseTexture();
		}
		
		meshList[i]->RenderMesh();
	}
}

void Model::LoadModel(const std::string& fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs |
		aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices); // aiProcess_MakeLeftHanded

	if (!scene) {
		std::cout << fileName << " Failed to load: " << importer.GetErrorString() << std::endl;
		return;
	}

	LoadNode(scene->mRootNode, scene);
	LoadMaterials(scene);
	
}

void Model::LoadNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

std::pair<std::vector<GLfloat>, std::vector<unsigned int>> Model::ProcessMesh(aiMesh* mesh) {
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		// Position
		vertices.insert(vertices.end(), {
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
			});

		// Texture Coordinates
		if (mesh->HasTextureCoords(0)) {
			/*std::cout << "UV: x " << mesh->mTextureCoords[0][i].x
				<< " UV: y " << mesh->mTextureCoords[0][i].y << std::endl*/;

			vertices.insert(vertices.end(), {
				mesh->mTextureCoords[0][i].x,    // U coordinate
				1.0f - mesh->mTextureCoords[0][i].y // Flip V coordinate
				});
		}
		else {
			// Planar mapping fallback (XZ plane)
			vertices.insert(vertices.end(), {
				mesh->mVertices[i].x, // U coordinate 
				mesh->mVertices[i].z  // V coordinate
				});
			std::cout << "Default UVs generated from position: "
				<< mesh->mVertices[i].x << ", "
				<< mesh->mVertices[i].z << std::endl;
		}

		// Normals
		vertices.insert(vertices.end(), {
			-mesh->mNormals[i].x,
			-mesh->mNormals[i].y,
			-mesh->mNormals[i].z
			});
	}
	

	// Extract indices
	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	return { vertices, indices };
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	auto [vertices, indices] = ProcessMesh(mesh);

	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	meshList.push_back(newMesh);
	meshToTex.push_back(mesh->mMaterialIndex);

}

void Model::LoadMaterials(const aiScene* scene)
{
	textureList.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial* material = scene->mMaterials[i];
		textureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
				// Extract filename from path
				std::string pathString = path.C_Str(); // Convert aiString to std::string
				size_t lastSlash = pathString.find_last_of("/\\"); // Handle both '/' and '\\'
				std::string filename = pathString.substr(lastSlash + 1);

				// Prepend directory if needed
				std::string texPath =  filename;

				// Create and load the texture
				textureList[i] = new Texture(texPath.c_str());
				if (!textureList[i]->LoadTexture()) {
					std::cerr << "Failed to load texture at: " << texPath << std::endl;
					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}

		// Fallback to default texture
		if (!textureList[i]) {
			textureList[i] = new Texture("default_1.jpg"); // Default texture
			if (!textureList[i]->LoadTexture()) {
				std::cerr << "Failed to load default texture: default_1.jpg" << std::endl;
				delete textureList[i];
				textureList[i] = nullptr;
			}
		}
	}
		
}

void Model::ClearModel()
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		if (meshList[i])
		{
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}

	for (size_t i = 0; i < textureList.size(); i++)
	{
		if (textureList[i])
		{
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
}

Model::~Model()
{
	ClearModel();
}
