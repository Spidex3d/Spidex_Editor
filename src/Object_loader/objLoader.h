#pragma once
#include <glad\glad.h>
#include <glm\glm.hpp>
#include <vector>
#include <string>

#include "../Ecs/BaseModel.h"
#include "../Headers/GlobalVars.h"

#include <stb\stb_image.h>

#include <iostream>
#include <sstream>
#include <fstream>




// we do need the basemode
class objLoader : public BaseModel
{
public:

	struct Material {  // NEW MTL
		std::string name;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;
		std::string diffuseMap;
		GLuint textureID;
		Material() : ambient(0.2f, 0.2f, 0.2f), diffuse(0.8f, 0.8f, 0.8f), specular(1.0f, 1.0f, 1.0f),
			shininess(32.0f), textureID(0) { }
	};
		
	
	

	void Initialize();
	// load obj files
	objLoader(int idx, const std::string& name, int ModleObjidx); // Default constructor
	std::vector<std::string> split(const std::string& s, const std::string& delimiter);
	bool Loadobj(const std::string& filename); // get the obj file name & path
	bool LoadMTL(const std::string& filename); // NEW MTL
	bool LoadObjTexture(const std::string& filename, GLuint& textureID);  // NEW MTL
	void objModels();
	void objDrawModels();

	

	~objLoader() {} // Private destructor

	private:
	GLuint VBO, VAO, EBO;

	int idx;
	const std::string& name;
	int ModleObjidx;  // obj files
	int glTFModelIndex; // gltf files

	bool m_Loaded;

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
	};
	std::vector<Vertex> m_Vertices;

	std::unordered_map<std::string, Material> materials;  // new for MTL

};

