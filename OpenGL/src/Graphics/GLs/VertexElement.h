#pragma once
#include "Vector.h"
#include "Matrix.h"
#include "VertexBufferLayout.h"

#define VERTEX_COMP Graphics::VertexBufferComponent::
#define VERTEX_STRUCT_LAYOUT inline static const Graphics::VertexBufferLayout __VERTEX_LAYOUT
#define VERTEX_LAYOUT_OF(T) T::__VERTEX_LAYOUT

struct VertexColorTexture3D {
    Maths::fvec3 Position;
    Maths::fvec4 Color;
    Maths::fvec2 TextureCoordinate;
    int TextureID = 0;
    
    VERTEX_STRUCT_LAYOUT = { VERTEX_COMP Vec3(), VERTEX_COMP Vec4(), VERTEX_COMP Vec2(), VERTEX_COMP Int() };
};

struct VertexColorTextureAtlas3D {
    Maths::fvec3 Position;
    Maths::fvec4 Color;
    int TextureAtlID = 0;
    int TextureCorner = 0;

    VERTEX_STRUCT_LAYOUT = { VERTEX_COMP Vec3(), VERTEX_COMP Vec4(), VERTEX_COMP Int(), VERTEX_COMP Int() };
};

struct VertexColor3D {
    Maths::fvec3 Position;
    Maths::fvec4 Color;

    VERTEX_STRUCT_LAYOUT = { VERTEX_COMP Vec3(), VERTEX_COMP Vec4() };
};

inline VertexColor3D operator*(const Maths::mat3D& transform, const VertexColor3D& vec) {
    return { transform * vec.Position, vec.Color };
}

inline VertexColorTexture3D operator*(const Maths::mat3D& transform, const VertexColorTexture3D& vec) {
    return { transform * vec.Position, vec.Color, vec.TextureCoordinate, vec.TextureID };
}

inline VertexColorTextureAtlas3D operator*(const Maths::mat3D& transform, const VertexColorTextureAtlas3D& vec) {
    return { transform * vec.Position, vec.Color, vec.TextureAtlID, vec.TextureCorner };
}