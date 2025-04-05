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
	std::string materialFile;

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
			else if (cmd == "mtllib") { // new for MTL file
				ss >> materialFile;
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
		 
		if (!materialFile.empty()) {
			LoadMTL(materialFile);
		}
		return (m_Loaded = true);
	}
	return false;
}
bool objLoader::LoadMTL(const std::string& filename)
{
	std::string folderpath = "Assets/Models/";
	std::string fullpath = folderpath + filename;
	std::ifstream fin(fullpath, std::ios::in);
	if (!fin.is_open()) {
		std::cout << "Cannot open MTL file " << fullpath << std::endl;
		return false;
	}
		std::cout << "Loading MTL file OK " << fullpath << std::endl;

		std::string lineBuffer;
		Material* currentMatirial = nullptr;

		while (std::getline(fin, lineBuffer)) {
			std::stringstream ss(lineBuffer);
			std::string cmd;
			ss >> cmd;
	
			if (cmd == "newmtl") {
				std::string matirialName;
				ss >> matirialName;
				materials[matirialName] = Material();
				currentMatirial = &materials[matirialName];
				currentMatirial->name = matirialName;
			}
			else if (cmd == "Ka" && currentMatirial) {
				ss >> currentMatirial->ambient.r >> currentMatirial->ambient.g >> currentMatirial->ambient.b;
 			}
			else if (cmd == "Kd" && currentMatirial) {
				ss >> currentMatirial->diffuse.r >> currentMatirial->diffuse.g >> currentMatirial->diffuse.b;
			}
			else if (cmd == "Ks" && currentMatirial) {
				ss >> currentMatirial->specular.r >> currentMatirial->specular.g >> currentMatirial->specular.b;
			}
			else if (cmd == "Ns" && currentMatirial) {
				ss >> currentMatirial->shininess;
			}
			else if (cmd == "map_Kd" && currentMatirial) {
				ss >> currentMatirial->diffuseMap;
				LoadObjTexture(folderpath + currentMatirial->diffuseMap, currentMatirial->textureID);
			}
			
		}

	fin.close();
	return true;
}
bool objLoader::LoadObjTexture(const std::string& filename, GLuint& textureID)
{
	std::cout << "MTL file found Loaded " << filename << std::endl;

	glGenTextures(1, &textureID);
	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

	if (data) {
		GLenum i_format;
		if (nrComponents == 1)  
			i_format = GL_RED;
		else if (nrComponents == 3) // jpg
			i_format = GL_RGB;
		else if (nrComponents == 4) // png
			i_format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, i_format, width, height, 0, i_format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << filename << std::endl;
		stbi_image_free(data);
	}

	return textureID;
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

	// Bind the texture for each material (assuming there is only one material for simplicity)
	for (const auto& materialPair : materials) { // matirials
		const Material& material = materialPair.second;
		if (material.textureID != 0) {
			glBindTexture(GL_TEXTURE_2D, material.textureID);
		}
	}

	//glBindTexture(GL_TEXTURE_2D, matirials["matirialName"].textureID);
	glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
// clean up the mess



