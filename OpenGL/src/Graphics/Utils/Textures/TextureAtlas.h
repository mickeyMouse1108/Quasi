#pragma once
#include "Texture.h"
#include "Vector.h"

namespace Graphics {
    class TextureAtlas {
        private:
            Texture texture;
            Maths::Vec2UInt atlasSize;
            Maths::Vec2UInt subTextureSize;
        public:
            TextureAtlas(const std::string& filename, uint xSize, uint ySize);

            [[nodiscard]] Maths::Vec2UInt GetTextureSize()    const { return { texture.GetWidth(), texture.GetHeight() }; }
            [[nodiscard]] Maths::Vec2UInt GetAtlasSize()      const { return atlasSize; }
            [[nodiscard]] Maths::Vec2UInt GetSubTextureSize() const { return subTextureSize; }

            void Bind(unsigned int slot = 0) const { texture.Bind(slot); }
            void Unbind()                    const { texture.Unbind(); }
    };
}
