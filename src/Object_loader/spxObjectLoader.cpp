#include "spxObjectLoader.h"
#include <iostream>
#include <sstream>
#include <fstream>

std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
	size_t start = 0;
	size_t end = s.find(delimiter);
	std::vector<std::string> tokens;
	while (end != std::string::npos) {
		tokens.push_back(s.substr(start, end - start));
		start = end + delimiter.length();
		end = s.find(delimiter, start);
	}
	tokens.push_back(s.substr(start, end));
	return tokens;
}

spxObjectLoader::spxObjectLoader() : m_Loaded(false)
{

}

spxObjectLoader::~spxObjectLoader()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

spxObjectLoader* spxObjectLoader::Instance()
{
	static spxObjectLoader objModel;
	return &objModel;
}

bool spxObjectLoader::loadOBJ(const std::string& filename)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;

	if (filename.find(".obj") != std::string::npos)
	{
		std::ifstream fin(filename, std::ios::in);
		if (!fin.is_open())
		{
			std::cerr << "Cannot Open " << filename << std::endl;
			return false;

		}

		std::cout << "Loding OBJfile " << filename << "..." << std::endl;

		std::string lineBuffer;
		while (std::getline(fin, lineBuffer))
		{
			std::stringstream ss(lineBuffer);
			std::string cmd;
			ss >> cmd;
			//################################ vertices ###########
			if (cmd == "v")
			{

				glm::vec3 vertex;
				int dim = 0;
				while (dim < 3 && ss >> vertex[dim])
					dim++;
				tempVertices.push_back(vertex);
			}
			//######################## texture #########################
			else if (cmd == "vt")
			{

				glm::vec2 uv;
				int dim = 0;
				while (dim < 2 && ss >> uv[dim])
					dim++;
				tempUVs.push_back(uv);
			}
			//########################## normals ############################
			else if (cmd == "vn")
			{

				glm::vec3 normal;
				int dim = 0;
				while (dim < 3 && ss >> normal[dim])
					dim++;

				normal = glm::normalize(normal);
				tempNormals.push_back(normal);
			}
			// ######################## Faces ######################
			else if (cmd == "f")
			{
				std::string faceData;
				int vertexIndex, uvIndex, normalIndex;
				while (ss >> faceData)
				{
					std::vector<std::string> data = split(faceData, "/");

					if (data[0].size() > 0)
						//if (data.size() > 0)
					{
						sscanf_s(data[0].c_str(), "%d", &vertexIndex);
						vertexIndices.push_back(vertexIndex);
					}
					// if the face has tex coords
					if (data.size() >= 1) {
						if (data[1].size() > 0)
						{
							sscanf_s(data[1].c_str(), "%d", &uvIndex);
							uvIndices.push_back(uvIndex);

						}
					}
					if (data.size() >= 2) {
						if (data[2].size() > 0)
						{
							sscanf_s(data[2].c_str(), "%d", &normalIndex);
							normalIndices.push_back(normalIndex);

						}
					}

				}


			}
			//std::cout << " Failed to parse the OBJ File" << std::endl;
		}
		fin.close();

		for (unsigned int i = 0; i < vertexIndices.size(); i++)
		{
			Vertex meshVertex;
			if (tempVertices.size() > 0) {
				glm::vec3 vertex = tempVertices[vertexIndices[i] - 1];
				meshVertex.position = vertex;
			}
			if (tempNormals.size() > 0) {
				glm::vec3 normal = tempNormals[normalIndices[i] - 1];
				meshVertex.normal = normal;
			}
			if (tempUVs.size() > 0) {
				glm::vec2 uv = tempUVs[uvIndices[i] - 1];
				meshVertex.texCoords = uv;

			}

			m_Vertices.push_back(meshVertex);
		}
		
		spxObjectLoader::initBuffers(idx, name, ModleObjidx);

		return (m_Loaded = true);
	}


	return false;

}
void spxObjectLoader::initBuffers(int idx, const std::string& name, int ModleObjidx) // this is for obj models
{
	index = idx;             // list index
	objectName = name;       // cube name
	objectIndex = Cubeobjidx;         // start index for a cube
	objectTypeID = OBJ_OBJ_MODEL; // Example type ID for Obj model set in GlobalVars = 12



	position = glm::vec3(0.0f, 0.0f, 0.0f); // Initial position
	scale = glm::vec3(1.0f, 1.0f, 1.0f);    // Initial scale
	modelMatrix = glm::mat4(1.0f);

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// tex Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void spxObjectLoader::objDraw()
{
	if (!m_Loaded) return;

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
	glBindVertexArray(0);

}

