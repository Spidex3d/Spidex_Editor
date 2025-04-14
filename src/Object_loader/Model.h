#pragma once
#include "../Shader/Shader.h"
#include "../Ecs/BaseModel.h"
#include "../Headers/GlobalVars.h"
#include <json\json.hpp>



using json = nlohmann::json;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Model : public BaseModel
{
public:
	// load gltf files
	Model(int inx, const std::string& name, int glTFModelIndex);
	~Model();

	void LoadglTF(const std::string filename);

	void DrawModel(Shader& shader);

	

private:
	GLuint VBO, EBO, VAO; 

	int idx;
	const std::string& name;
	int glTFModelIndex;
	json JSON;

	std::vector<unsigned char> getData();
	std::vector<float> getFloat(json accessor);
	std::vector<GLuint> getIndices(json accessor);

	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);


};

