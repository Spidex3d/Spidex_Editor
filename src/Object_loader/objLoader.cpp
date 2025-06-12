#include "objLoader.h"



// initialize stuff if we need to
void objLoader::Initialize()
{
	
}
objLoader::objLoader(int idx, const std::string& name, int ModleObjidx)
	: idx(idx), name(name), ModleObjidx(ModleObjidx) {
	index = idx;             // list index
	objectName = name;       // cube name
	objectIndex = ModleObjidx;         // start index for a cube
	objectTypeID = OBJ_OBJ_MODEL; // Example type ID for Cube set in config = 1

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
// Load obj file New and Improved
 


 

 
bool objLoader::Loadobj(const std::string& filename)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;
	std::string materialFile;

	if (filename.find(".obj") == std::string::npos)
		return false;

	std::ifstream fin(filename);
	if (!fin.is_open()) {
		std::cerr << "Cannot open " << filename << std::endl;
		return false;
	}

	std::cout << "Loading OBJ file " << filename << "..." << std::endl;

	std::string line;
	while (std::getline(fin, line)) {
		std::stringstream ss(line);
		std::string cmd;
		ss >> cmd;

		if (cmd == "v") {
			glm::vec3 vertex;
			ss >> vertex.x >> vertex.y >> vertex.z;
			tempVertices.push_back(vertex);
		}
		else if (cmd == "vt") {
			glm::vec2 uv;
			ss >> uv.x >> uv.y;
			tempUVs.push_back(uv);
		}
		else if (cmd == "vn") {
			glm::vec3 normal;
			ss >> normal.x >> normal.y >> normal.z;
			tempNormals.push_back(glm::normalize(normal));
		}
		else if (cmd == "f") {
			std::vector<std::string> tokens;
			std::string token;
			while (ss >> token) tokens.push_back(token);

			if (tokens.size() < 3) continue; // not enough for a triangle

			std::vector<unsigned int> vIdxs, vtIdxs, vnIdxs;
			for (const auto& faceData : tokens) {
				int v = 0, vt = 0, vn = 0;
				if (sscanf_s(faceData.c_str(), "%d/%d/%d", &v, &vt, &vn) == 3) {
					// v/vt/vn
				}
				else if (sscanf_s(faceData.c_str(), "%d//%d", &v, &vn) == 2) {
					// v//vn
				}
				else if (sscanf_s(faceData.c_str(), "%d/%d", &v, &vt) == 2) {
					// v/vt
				}
				else if (sscanf_s(faceData.c_str(), "%d", &v) == 1) {
					// v
				}

				// Handle negative indices
				if (v < 0) v += tempVertices.size() + 1;
				if (vt < 0) vt += tempUVs.size() + 1;
				if (vn < 0) vn += tempNormals.size() + 1;

				vIdxs.push_back(v);
				vtIdxs.push_back(vt);
				vnIdxs.push_back(vn);
			}

			// Triangulate face (triangle fan method)
			for (size_t i = 1; i < vIdxs.size() - 1; ++i) {
				unsigned int v[] = { vIdxs[0], vIdxs[i], vIdxs[i + 1] };
				unsigned int vt[] = { vtIdxs[0], vtIdxs[i], vtIdxs[i + 1] };
				unsigned int vn[] = { vnIdxs[0], vnIdxs[i], vnIdxs[i + 1] };

				for (int j = 0; j < 3; ++j) {
					Vertex meshVertex;

					if (v[j] > 0 && v[j] <= tempVertices.size())
						meshVertex.position = tempVertices[v[j] - 1];
					if (vn[j] > 0 && vn[j] <= tempNormals.size())
						meshVertex.normal = tempNormals[vn[j] - 1];
					if (vt[j] > 0 && vt[j] <= tempUVs.size())
						meshVertex.texCoords = tempUVs[vt[j] - 1];

					m_Vertices.push_back(meshVertex);
				}
			}
		}
		else if (cmd == "mtllib") {
			ss >> materialFile;
		}
	}

	fin.close();

	if (!materialFile.empty()) {
		LoadMTL(materialFile);
	}

	return (m_Loaded = true);
}
 
bool objLoader::LoadMTL(const std::string& filename)
{
	std::string fullpath = modelPath + filename;
	std::ifstream fin(fullpath);
	if (!fin.is_open()) {
		std::cerr << "Cannot open MTL file: " << fullpath << std::endl;
		return false;
	}

	std::cout << "Loading MTL file: " << fullpath << std::endl;

	std::string lineBuffer;
	Material* currentMaterial = nullptr;

	while (std::getline(fin, lineBuffer)) {
		std::stringstream ss(lineBuffer);
		std::string cmd;
		ss >> cmd;

		if (cmd == "newmtl") {
			std::string materialName;
			ss >> materialName;
			materials[materialName] = Material();
			currentMaterial = &materials[materialName];
			currentMaterial->name = materialName;
		}
		else if (cmd == "Ka" && currentMaterial) {
			ss >> currentMaterial->ambient.r >> currentMaterial->ambient.g >> currentMaterial->ambient.b;
		}
		else if (cmd == "Kd" && currentMaterial) {
			ss >> currentMaterial->diffuse.r >> currentMaterial->diffuse.g >> currentMaterial->diffuse.b;
		}
		else if (cmd == "Ks" && currentMaterial) {
			ss >> currentMaterial->specular.r >> currentMaterial->specular.g >> currentMaterial->specular.b;
		}
		else if (cmd == "Ns" && currentMaterial) {
			ss >> currentMaterial->shininess;
		}
		else if ((cmd == "d" || cmd == "Tr") && currentMaterial) {
			ss >> currentMaterial->transparency;
		}
		else if (cmd == "Ni" && currentMaterial) {
			ss >> currentMaterial->opticalDensity;
		}
		else if (cmd == "illum" && currentMaterial) {
			ss >> currentMaterial->illumModel;
		}
		else if (cmd == "map_Kd" && currentMaterial) {
			ss >> currentMaterial->diffuseMap;
			//LoadObjTexture(modelPath + currentMaterial->diffuseMap, currentMaterial->textureID);
			//LoadObjTexture(modelPath + currentMaterial->diffuseMap, currentMaterial->diffuseTexID);
			currentMaterial->diffuseTexID = LoadObjTexture(modelPath + currentMaterial->diffuseMap);
		}
		else if (cmd == "map_Ks" && currentMaterial) {
			ss >> currentMaterial->specularMap;
			//LoadObjTexture(modelPath + currentMaterial->specularMap, currentMaterial->specularMapID);
			//LoadObjTexture(modelPath + currentMaterial->specularMap, currentMaterial->specularTexID);
			currentMaterial->specularTexID = LoadObjTexture(modelPath + currentMaterial->specularMap);
		}
		else if ((cmd == "map_bump" || cmd == "bump") && currentMaterial) {
			ss >> currentMaterial->normalMap;
			//LoadObjTexture(modelPath + currentMaterial->normalMap, currentMaterial->normalMapID);
			//LoadObjTexture(modelPath + currentMaterial->normalMap, currentMaterial->normalTexID);
			currentMaterial->normalTexID = LoadObjTexture(modelPath + currentMaterial->normalMap);
		}
		else if (cmd == "map_d" && currentMaterial) {
			ss >> currentMaterial->alphaMap;
			//LoadObjTexture(modelPath + currentMaterial->alphaMap, currentMaterial->alphaTexID);
			currentMaterial->alphaTexID = LoadObjTexture(modelPath + currentMaterial->alphaMap);
		}
		
	}

	if (currentMaterial) {
		if (!currentMaterial->diffuseMap.empty() || !currentMaterial->specularMap.empty()
			|| !currentMaterial->normalMap.empty() || !currentMaterial->alphaMap.empty()) {
			LoadMaterialTextures(*currentMaterial, modelPath);
		}
	}

	fin.close();
	return true;
}

//bool objLoader::LoadObjTexture(const std::string& filename)  // GLuint& textureID
GLuint objLoader::LoadObjTexture(const std::string& filename)
{
	std::cout << "MTL file found Loaded " << filename << std::endl;
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

	if (data) {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		else
			format = GL_RGB; // fallback

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
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
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
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

	glBindVertexArray(VAO);
		
	for (const auto& materialPair : materials) { // matirials
		const Material& material = materialPair.second;
		if (material.diffuseTexID != 0) {  //textureID
			glBindTexture(GL_TEXTURE_2D, material.diffuseTexID); // textureID
		}
		/*if (material.diffuseTexID != 0) {
			std::cout << "Binding texture ID: " << material.diffuseTexID << " for material: " << material.name << std::endl;
			glBindTexture(GL_TEXTURE_2D, material.diffuseTexID);
		}*/
	}

	glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void objLoader::LoadMaterialTextures(Material& mat, const std::string& filename)
{
	if (!mat.diffuseMap.empty())
		mat.diffuseTexID = LoadObjTexture(filename + mat.diffuseMap);

	if (!mat.specularMap.empty())
		mat.specularTexID = LoadObjTexture(filename + mat.specularMap);

	if (!mat.normalMap.empty())
		mat.normalTexID = LoadObjTexture(filename + mat.normalMap);

	if (!mat.alphaMap.empty())
		mat.alphaTexID = LoadObjTexture(filename + mat.alphaMap);
}





