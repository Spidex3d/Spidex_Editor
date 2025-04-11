#pragma once
#include <glad\glad.h>
#include <glm\glm.hpp>
#include <vector>
#include <string>

#include "../Ecs/BaseModel.h"
#include "../Headers/GlobalVars.h"
#include "../Shader/Shader.h"
#include <stb\stb_image.h>

#include <iostream>
#include <sstream>
#include <fstream>
	
// Assimp test
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	float m_Wwights[MAX_BONE_INFLUENCE];
};
struct Texture {
	unsigned int id;
	std::string type;
	std::string path;

};


// we do need the basemode  : public BaseModel
class Mesh
{
public:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;
	unsigned int  m_EBO;

	

	// get the models mesh
	Mesh(std::vector<Vertex> m_vertices, std::vector<unsigned int> m_indices,
		std::vector<Texture> m_textures);
		


	
	~Mesh() {} // destructor

	void setupMesh(); // get the Model file name & path

	void DrawModels(Shader &shader);

	private:
		GLuint m_VBO, m_VAO;
		int idx;
		//const std::string& name;
		int ModleObjidx;
		bool m_Loaded;	

};

