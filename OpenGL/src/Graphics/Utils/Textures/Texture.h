#pragma once
#include <string>
#include "opengl.h"

#include "NumTypes.h"

namespace Graphics {
    class Texture {
    private:
        glID rendererID = GL_NULL;
        int width = 0, height = 0, BPPixel = 0; //stands for bits per pixel

        OPENGL_API void LoadTexture(const uchar* img, bool useLinear = true);
     public:
        OPENGL_API Texture() = default;
        OPENGL_API explicit Texture(const uchar* dat, int len, bool useLinear = true);
        OPENGL_API explicit Texture(const std::string& filePath, bool useLinear = true);
        OPENGL_API ~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;
        OPENGL_API static void Transfer(Texture& dest, Texture&& from);
        Texture(Texture&& tex) noexcept { Transfer(*this, std::move(tex)); }
        Texture& operator=(Texture&& tex) noexcept { Transfer(*this, std::move(tex)); return *this; }

        OPENGL_API void Bind(uint slot = 0) const;
        OPENGL_API void Unbind() const;

        unsigned int GetWidth() const { return width; }
        unsigned int GetHeight() const { return height; }
    };
}
