#include "Texture.h"

#include <stdexcept>

// This class is just for the Assimp Model loder

Texture::Texture()
{
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = "";
}

Texture::Texture(const char* imageFile)
{
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = imageFile;
    
}

bool Texture::LoadTexture()
{
    //stbi_set_flip_vertically_on_load(true);

    unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
    if (!texData)
    {
        printf("Failed to find: %s\n", fileLocation);
        return false;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(texData);

    return true;
}
bool Texture::LoadTextureA()
{
    //stbi_set_flip_vertically_on_load(true);

    unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
    if (!texData)
    {
        printf("Failed to find: %s\n", fileLocation);
        return false;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(texData);

    return true;
}

void Texture::UseTexture()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::ClearTexture()
{
    glDeleteTextures(1, &textureID);
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = "";

}

Texture::~Texture()
{
    ClearTexture();
}

//void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
//{
//    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
//    shader.Use();
//    glUniform1i(texUni, unit);
//}
//
//void Texture::Bind()
//{
//    glActiveTexture(GL_TEXTURE0 + unit);
//    glBindTexture(GL_TEXTURE_2D, ID);
//}
//
//void Texture::Unbind()
//{
//    glBindTexture(GL_TEXTURE_2D, 0);
//}
//
//void Texture::Delete()
//{
//    glDeleteTextures(1, &ID);
//}




//Texture::Texture(const char* image, const char* texType, GLuint slot)
//{
//	// Assigns the type of the texture ot the texture object
//		type = texType;
//	
//		// Stores the width, height, and the number of color channels of the image
//		int widthImg, heightImg, numColCh;
//		// Flips the image so it appears right side up
//		stbi_set_flip_vertically_on_load(true);
//		// Reads the image from a file and stores it in bytes
//		unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);
//		if (!bytes) {
//			throw std::runtime_error(std::string("Failed to load texture: ") + image);
//		}
//	
//		// Generates an OpenGL texture object
//		glGenTextures(1, &ID);
//		// Assigns the texture to a Texture Unit
//		glActiveTexture(GL_TEXTURE0 + slot);
//		unit = slot;
//		glBindTexture(GL_TEXTURE_2D, ID);
//	
//		// Configures the type of algorithm that is used to make the image smaller or bigger
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	
//		// Configures the way the texture repeats (if it does at all)
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	
//		// Extra lines in case you choose to use GL_CLAMP_TO_BORDER
//		// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
//		// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);
//	
//		// Check what type of color channels the texture has and load it accordingly
//		if (numColCh == 4) // png
//			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
//		else if (numColCh == 3) // jpg
//			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
//		else if (numColCh == 1) // grayscale
//			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RED, GL_UNSIGNED_BYTE, bytes);
//		else
//			throw std::invalid_argument("Automatic Texture type recognition failed");
//	
//		// Generates MipMaps
//		glGenerateMipmap(GL_TEXTURE_2D);
//	
//		// Deletes the image data as it is already in the OpenGL Texture object
//		stbi_image_free(bytes);
//	
//		// Unbinds the OpenGL Texture object so that it can't accidentally be modified
//		glBindTexture(GL_TEXTURE_2D, 0);
//}
//
//void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
//{
//		// Gets the location of the uniform
//	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
//	// Shader needs to be activated before changing the value of a uniform
//	shader.Use();
//	// Sets the value of the uniform
//	glUniform1i(texUni, unit);
//}
//
//void Texture::Bind()
//{
//	glActiveTexture(GL_TEXTURE0 + unit);
//	glBindTexture(GL_TEXTURE_2D, ID);
//}
//
//void Texture::Unbind()
//{
//	glBindTexture(GL_TEXTURE_2D, 0);
//}
//
//void Texture::Delete()
//{
//	glDeleteTextures(1, &ID);
//}
