#include "Texture.h"
#include "Debugging.h"
#include "../vendor/stb_image/stb_image.h"

namespace Graphics {
    Texture::Texture(const uchar* datpng, int len, bool useLinear)
        : rendererID(0) {
        //flips texture
        stbi_set_flip_vertically_on_load(1);
        uchar* localTexture = stbi_load_from_memory(datpng, len, &width, &height, &BPPixel, 4);
        LoadTexture(localTexture, useLinear);
    }

    Texture::Texture(const std::string& filePath, bool useLinear)
        : rendererID(0) {
        //flips texture
        stbi_set_flip_vertically_on_load(1);
        uchar* localTexture = stbi_load(filePath.c_str(), &width, &height, &BPPixel, 4);
        LoadTexture(localTexture, useLinear);
    }

    Texture::~Texture() {
        GLCALL(glDeleteTextures(1, &rendererID));
    }

    void Texture::LoadTexture(const uchar* img, bool useLinear) {
        GLCALL(glGenTextures(1, &rendererID));
        GLCALL(glBindTexture(GL_TEXTURE_2D, rendererID));

        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, useLinear ? GL_LINEAR : GL_NEAREST));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, useLinear ? GL_LINEAR : GL_NEAREST));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img));
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

        if (img) stbi_image_free((void*)img);
    }

    void Texture::Transfer(Texture& dest, Texture&& from) {
        dest.rendererID = from.rendererID;
        from.rendererID = 0;

        dest.width = from.width;
        dest.height = from.height;
        dest.BPPixel = from.BPPixel;
    }

    void Texture::Bind(uint slot/*default = 0*/) const {
        GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
        GLCALL(glBindTexture(GL_TEXTURE_2D, rendererID));
    }

    void Texture::Unbind() const {
        GLCALL(glBindTexture(GL_TEXTURE_2D, rendererID));
    }
}