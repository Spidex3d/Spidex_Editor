#pragma once
#include <glad\glad.h>
#include <glm\glm.hpp>
#include <vector>
#include <string>

#include "../Ecs/BaseModel.h"
#include "../Headers/GlobalVars.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};


class spxObjectLoader : public BaseModel
{
public:
	spxObjectLoader();
	~spxObjectLoader();

	static spxObjectLoader* Instance();

	bool loadOBJ(const std::string& filename);
	void objDraw();
	void initBuffers(int idx, const std::string& name, int ModleObjidx);

	//GLuint m_VBO, m_VAO;

private:
	int idx;
	const std::string name;
	int ModleObjidx;

	bool m_Loaded;
	std::vector<Vertex> m_Vertices;
	GLuint m_VBO, m_VAO;
};

