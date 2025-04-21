#pragma once
#include <glad/glad.h>
#include <stb/stb_image.h>
#include "../Shader/Shader.h"
#include "../Shader/ShaderManager.h"

// This class is just for the Assimp Model loder
class Texture
{
public:
	GLuint ID;
	const char* type;
	//GLuint unit;

	Texture();
	// Constructor
	Texture(const char* imageFile);

	bool LoadTexture();
	//bool LoadTextureA();

	void UseTexture();
	void ClearTexture();

	~Texture();

private:
	GLuint textureID;
	int width, height, nrComponents;
	//int width, height, bitDepth;

	const char* fileLocation;
};
