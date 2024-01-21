#include "Texture.h"
#include "Debugging.h"
#include "../vendor/stb_image/stb_image.h"

namespace Graphics {
    Texture::Texture(const uchar* raw, int w, int h, bool useLinear, int format)
        : rendererID(0), width(w), height(h) {
        //flips texture
        LoadTexture(raw, useLinear, format);
    }

    Texture::Texture(const std::string& filePath, bool useLinear)
        : rendererID(0) {
        //flips texture
        stbi_set_flip_vertically_on_load(1);
        const uchar* localTexture = stbi_load(filePath.c_str(), &width, &height, &BPPixel, 4);
        LoadTexture(localTexture, useLinear);
        if (localTexture) stbi_image_free((void*)localTexture);
    }

    Texture::~Texture() {
        GLCALL(glDeleteTextures(1, &rendererID));
    }

    void Texture::LoadTexture(const uchar* img, bool useLinear, int format) {
        GLCALL(glGenTextures(1, &rendererID));
        GLCALL(glBindTexture(GL_TEXTURE_2D, rendererID));

        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, useLinear ? GL_LINEAR : GL_NEAREST));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, useLinear ? GL_LINEAR : GL_NEAREST));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, img));
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
    }

    void Texture::Transfer(Texture& dest, Texture&& from) {
        dest.rendererID = from.rendererID;
        from.rendererID = 0;

        dest.width = from.width;
        dest.height = from.height;
        dest.BPPixel = from.BPPixel;
    }

    Texture Texture::LoadPNGBytes(const uchar* png, int len, bool useLinear) {
        Texture tex;
        
        stbi_set_flip_vertically_on_load(1);
        const uchar* localTexture = stbi_load_from_memory(png, len, &tex.width, &tex.height, &tex.BPPixel, 4);
        tex.LoadTexture(localTexture, useLinear);
        if (localTexture) stbi_image_free((void*)localTexture);
        
        return tex;
    }

    void Texture::Bind(uint slot/*default = 0*/) const {
        GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
        GLCALL(glBindTexture(GL_TEXTURE_2D, rendererID));
    }

    void Texture::Unbind() const {
        GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
    }
}