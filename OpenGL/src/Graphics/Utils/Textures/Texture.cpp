#include "Texture.h"
#include "Debugging.h"
#include "../vendor/stb_image/stb_image.h"

namespace Graphics
{
    Texture::Texture(const std::string& filePath, bool useLinear)
        : rendererID(0), filePath(filePath), localTexture(nullptr), width(0), height(0), BPPixel(0)
    {
        //flips texture
        stbi_set_flip_vertically_on_load(1);
        localTexture = stbi_load(filePath.c_str(), &width, &height, &BPPixel, 4);
    
        GLCALL(glGenTextures(1, &rendererID));
        GLCALL(glBindTexture(GL_TEXTURE_2D, rendererID));

        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, useLinear ? GL_LINEAR : GL_NEAREST));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, useLinear ? GL_LINEAR : GL_NEAREST));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localTexture));
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

        if (localTexture) stbi_image_free(localTexture);
    }

    Texture::~Texture()
    {
        GLCALL(glDeleteTextures(1, &rendererID));
    }

    void Texture::Bind(unsigned int slot/*default = 0*/) const
    {
        GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
        GLCALL(glBindTexture(GL_TEXTURE_2D, rendererID));
    }

    void Texture::Unbind() const
    {
        GLCALL(glBindTexture(GL_TEXTURE_2D, rendererID));
    }
}