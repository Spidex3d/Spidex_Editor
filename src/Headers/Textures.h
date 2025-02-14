#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <glad\glad.h>

#include <iostream>
#include <stb\stb_image.h>

unsigned int loadTexture(const std::string& filePath) // use this one
{

    unsigned int textureID;

    glGenTextures(1, &textureID);

    int width, height, nrComponents;

    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrComponents, 0); // use this one
    if (data)
    {
        GLenum i_format;

        if (nrComponents == 1)
            i_format = GL_RED;
        else if (nrComponents == 3) // jpg file
            i_format = GL_RGB;
        else if (nrComponents == 4) // png file
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
    else
    {
        std::cout << "Texture failed to load at path: " << filePath << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
