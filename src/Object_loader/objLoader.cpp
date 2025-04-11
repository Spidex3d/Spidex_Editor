#include "objLoader.h"


// initialize stuff if we need to
void objLoader::Initialize()
{
	
}
objLoader::objLoader(int idx, const std::string& name, int ModleObjidx)
	: idx(idx), name(name), ModleObjidx(ModleObjidx) {
}

std::vector<std::string> objLoader::split(const std::string& s, const std::string& delimiter)
{
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
// Load obj file
bool objLoader::Loadobj(const std::string& filename)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;

	if (filename.find(".obj") != std::string::npos) {
		std::ifstream fin(filename, std::ios::in);
		if (!fin.is_open()) {
			std::cerr << "Cannot Open " << filename << std::endl;
			return false;
		}

		std::cout << "Loading OBJ file " << filename << "..." << std::endl;

		std::string lineBuffer;
		while (std::getline(fin, lineBuffer)) {
			std::stringstream ss(lineBuffer);
			std::string cmd;
			ss >> cmd;

			if (cmd == "v") {
				glm::vec3 vertex;
				int dim = 0;
				while (dim < 3 && ss >> vertex[dim])
					dim++;
				tempVertices.push_back(vertex);
			}
			else if (cmd == "vt") {
				glm::vec2 uv;
				int dim = 0;
				while (dim < 2 && ss >> uv[dim])
					dim++;
				tempUVs.push_back(uv);
			}
			else if (cmd == "vn") {
				glm::vec3 normal;
				int dim = 0;
				while (dim < 3 && ss >> normal[dim])
					dim++;
				normal = glm::normalize(normal);
				tempNormals.push_back(normal);
			}
			else if (cmd == "f") {
				std::string faceData;
				int vertexIndex, uvIndex, normalIndex;
				while (ss >> faceData) {
					std::vector<std::string> data = split(faceData, "/");

					if (data[0].size() > 0) {
						sscanf_s(data[0].c_str(), "%d", &vertexIndex);
						vertexIndices.push_back(vertexIndex);
					}
					if (data.size() >= 1 && data[1].size() > 0) {
						sscanf_s(data[1].c_str(), "%d", &uvIndex);
						uvIndices.push_back(uvIndex);
					}
					if (data.size() >= 2 && data[2].size() > 0) {
						sscanf_s(data[2].c_str(), "%d", &normalIndex);
						normalIndices.push_back(normalIndex);
					}
				}
			}
		}
		fin.close();

		for (unsigned int i = 0; i < vertexIndices.size(); i++) {
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
		 
		//objLoader::objModels(mesh[], size);
		return (m_Loaded = true);
	}
	return false;
}
// set up the obj vertices and stuff
//void objLoader::objModels(int mesh[], int size)
void objLoader::objModels()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}
// draw the models
void objLoader::objDrawModels()
{
	if (!m_Loaded) return;

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
	glBindVertexArray(0);
}
// clean up the mess



