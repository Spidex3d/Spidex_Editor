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

	struct Material {
		std::string name;

		glm::vec3 ambient;      // Ka
		glm::vec3 diffuse;      // Kd
		glm::vec3 specular;     // Ks
		float shininess;        // Ns

		float transparency;     // d or Tr
		float opticalDensity;   // Ni
		int illumModel;         // illum

		// Texture maps
		std::string diffuseMap;     // map_Kd
		std::string specularMap;    // map_Ks
		std::string normalMap;      // map_bump or bump
		std::string alphaMap;       // map_d or map_opacity

		GLuint diffuseTexID = 0;
		GLuint specularTexID = 0;
		GLuint normalTexID = 0;
		GLuint alphaTexID = 0;

		Material()
			: ambient(0.2f), diffuse(0.8f), specular(1.0f),
			shininess(32.0f), transparency(1.0f), opticalDensity(1.0f), illumModel(2) {
		}
	};

		
	
	

	void Initialize();
	// load obj files
	objLoader(int idx, const std::string& name, int ModleObjidx); // Default constructor
	std::vector<std::string> split(const std::string& s, const std::string& delimiter);
	bool Loadobj(const std::string& filename); // get the obj file name & path
	bool LoadMTL(const std::string& filename); // NEW MTL
	//bool LoadObjTexture(const std::string& filename);  // NEW MTL    GLuint& textureID
	GLuint LoadObjTexture(const std::string& filename);  // NEW MTL    GLuint& textureID
	void objModels();
	void objDrawModels();
	void LoadMaterialTextures(Material& mat, const std::string& filename);
	

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

