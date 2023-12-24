#pragma once
#include <string>
#include "opengl.h"

#include "NumTypes.h"

namespace Graphics {
    class Texture {
    private:
        unsigned int rendererID;
        int width, height, BPPixel; //stands for bits per pixel

        OPENGL_API void LoadTexture(const uchar* img, bool useLinear = true);
     public:
        OPENGL_API Texture(const uchar* dat, int len, bool useLinear = true);
        OPENGL_API Texture(const std::string& filePath, bool useLinear = true);
        OPENGL_API ~Texture();

        OPENGL_API void Bind(unsigned int slot = 0) const;
        OPENGL_API void Unbind() const;

        unsigned int GetWidth() const { return width; }
        unsigned int GetHeight() const { return height; }
    };
}
