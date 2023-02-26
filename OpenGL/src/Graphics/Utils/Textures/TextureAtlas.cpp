#include "TextureAtlas.h"

namespace Graphics {
    TextureAtlas::TextureAtlas(const std::string& filename, uint xSize, uint ySize) :
        texture(filename), atlasSize({ xSize, ySize }), subTextureSize({ texture.GetWidth() / xSize, texture.GetHeight() / ySize }) {}
}
