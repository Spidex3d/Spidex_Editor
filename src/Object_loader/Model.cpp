#include "Model.h"

Model::Model(int idx, const std::string& name, int glTFModelIndex)
	:idx(idx), name(name), glTFModelIndex(glTFModelIndex)
{
	index = idx;
	objectName = name;
	objectIndex = glTFModelIndex;
	objectTypeID = GLTF_MODEL;




}

Model::~Model()
{

}
// loadModel
void Model::LoadglTF(const std::string filename)
{
	
}

void Model::DrawModel(Shader& shader)
{
}

std::vector<unsigned char> Model::getData()
{
	return std::vector<unsigned char>();
}

std::vector<float> Model::getFloat(json accessor)
{
	std::vector<float> floatVec;

	unsigned int bufferViewInd = accessor.value("bufferView", 1);
	unsigned int count = accessor["count"];
	unsigned int accByteOffset = accessor.value("byteOffset", 0);
	std::string type = accessor["type"];

	json bufferView = JSON["bufferViews"][bufferViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];

	unsigned int numPerVert;
	if (type == "SCALAR") numPerVert = 1;
	else if (type == "VEC2") numPerVert = 2;
	else if (type == "VEC3") numPerVert = 3;
	else if (type == "VEC4") numPerVert = 4;
	else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");


	return floatVec;
}

std::vector<GLuint> Model::getIndices(json accessor)
{
	return std::vector<GLuint>();
}

std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec)
{
	return std::vector<glm::vec2>();
}

std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec)
{
	return std::vector<glm::vec3>();
}

std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec)
{
	return std::vector<glm::vec4>();
}
