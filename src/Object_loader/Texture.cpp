#include "Texture.h"

#include <stdexcept>

// This class is just for the Assimp Model loder

Texture::Texture()
{
    /*textureID = 0;
    width = 0;
    height = 0;
    nrComponents = 0;
    fileLocation = "";*/
}

Texture::Texture(const char* imageFile)
{
    textureID = 0;
    width = 0;
    height = 0;
    nrComponents = 0;
    fileLocation = imageFile;
    
}

bool Texture::LoadTexture()
{
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(fileLocation, &width, &height, &nrComponents, 0);
    if (data)
    {

        std::cout << "Loaded texture: " << fileLocation << std::endl;
        std::cout << "Width: " << width << ", Height: " << height << ", Components: " << nrComponents << std::endl;

        GLenum i_format;

        if (nrComponents == 1)      // Grayscale format
            i_format = GL_RED;
        else if (nrComponents == 3) // RGB format(JPEG or RGB PNG)
            i_format = GL_RGB;
        else if (nrComponents == 4) // RGBA format (PNG with alpha)
            i_format = GL_RGBA;

        // Generate and bind the texture
        if (textureID == 0) {
            glGenTextures(1, &textureID);
        }
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, i_format, width, height, 0, i_format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture: " << fileLocation << " - " << stbi_failure_reason() << std::endl;
        stbi_image_free(data);
        return false;
    }

    return textureID;

}

void Texture::UseTexture()
{
    glActiveTexture(GL_TEXTURE0);
    //glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
}
void Texture::ClearTexture()
{
    glDeleteTextures(1, &textureID);
    textureID = 0;
    width = 0;
    height = 0;
    nrComponents = 0;
    fileLocation = "";

}

Texture::~Texture()
{
    ClearTexture();
}

