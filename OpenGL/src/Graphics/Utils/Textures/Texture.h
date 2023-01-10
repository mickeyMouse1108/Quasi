#pragma once
#include <string>

namespace Graphics
{
    class Texture
    {
    private:
        unsigned int rendererID;
        std::string filePath;
        unsigned char* localTexture;
        int width, height, BPPixel; //stands for bits per pixel
        public:
        Texture(const std::string& filePath, bool useLinear = true);
        ~Texture();

        void Bind(unsigned int slot = 0) const;
        void Unbind() const;

        unsigned int GetWidth() const { return width; }
        unsigned int GetHeight() const { return height; }
    };
}
