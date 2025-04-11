#pragma once
#include <glad\glad.h>
#include <glm\glm.hpp>
#include <vector>
#include <string>

#include "../Ecs/BaseModel.h"
#include "../Headers/GlobalVars.h"

#include <iostream>
#include <sstream>
#include <fstream>
// we do need the basemode
class objLoader : public BaseModel
{
public:
		
	objLoader(int idx, const std::string& name, int ModleObjidx); // Default constructor
	
	~objLoader() {} // Private destructor

	void Initialize();

	std::vector<std::string> split(const std::string& s, const std::string& delimiter);

	bool Loadobj(const std::string& filename); // get the obj file name & path

	void objModels();

	void objDrawModels();

	private:
	GLuint m_VBO, m_VAO;

	int idx;
	const std::string& name;
	int ModleObjidx;

	bool m_Loaded;

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
	};
	std::vector<Vertex> m_Vertices;

};

