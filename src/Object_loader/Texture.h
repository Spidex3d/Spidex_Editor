#pragma once
#include <glad/glad.h>
#include <stb/stb_image.h>
#include "../Shader/Shader.h"

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

	/*bool LoadTexture(const char* image, const char* texType,
		GLint wrapS = GL_REPEAT, GLint wrapT = GL_REPEAT,
		GLint minFilter = GL_NEAREST_MIPMAP_LINEAR, GLint magFilter = GL_NEAREST);*/
	bool LoadTexture();
	bool LoadTextureA();

	//bool LoadTextureA();

	void UseTexture();
	void ClearTexture();

	~Texture();

	//// Assigns a texture unit to a texture
	//void texUnit(Shader& shader, const char* uniform, GLuint unit);
	//// Binds a texture
	//void Bind();
	//// Unbinds a texture
	//void Unbind();
	//// Deletes a texture
	//void Delete();

private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;
};


/*
Texture();
	Texture(const char* fileLoc);

	bool LoadTexture();
	bool LoadTextureA();

	void UseTexture();
	void ClearTexture();

	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;
*/
