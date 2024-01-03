#pragma once
#include <string>
#include "opengl.h"

#include "NumTypes.h"

namespace Graphics {
    class Texture {
    private:
        uint rendererID;
        int width, height, BPPixel; //stands for bits per pixel

        OPENGL_API void LoadTexture(const uchar* img, bool useLinear = true);
     public:
        OPENGL_API Texture();
        OPENGL_API Texture(const uchar* dat, int len, bool useLinear = true);
        OPENGL_API Texture(const std::string& filePath, bool useLinear = true);
        OPENGL_API ~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;
        OPENGL_API static Texture& Transfer(Texture& dest, Texture&& from);
        Texture(Texture&& tex) { Transfer(*this, std::move(tex)); }
        Texture& operator=(Texture&& tex) { return Transfer(*this, std::move(tex)); }

        OPENGL_API void Bind(uint slot = 0) const;
        OPENGL_API void Unbind() const;

        unsigned int GetWidth() const { return width; }
        unsigned int GetHeight() const { return height; }
    };
}
