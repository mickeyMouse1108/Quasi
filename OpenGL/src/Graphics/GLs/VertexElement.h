#pragma once
#include "Vector.h"
#include "Matrix.h"

struct VertexColorTexture3D {
    Maths::Vector3 Position;
    Maths::Vector4 Color;
    Maths::Vector2 TextureCoordinate;
    int TextureID = 0;
};

struct VertexColorTextureAtlas3D {
    Maths::Vector3 Position;
    Maths::Vector4 Color;
    int TextureAtlID = 0;
    int TextureCorner = 0;
};

struct VertexColor3D {
    Maths::Vector3 Position;
    Maths::Vector4 Color;
};

inline VertexColor3D operator*(const Maths::Matrix3D& transform, const VertexColor3D& vec) {
    return { transform * vec.Position, vec.Color };
}

inline VertexColorTexture3D operator*(const Maths::Matrix3D& transform, const VertexColorTexture3D& vec) {
    return { transform * vec.Position, vec.Color, vec.TextureCoordinate, vec.TextureID };
}

inline VertexColorTextureAtlas3D operator*(const Maths::Matrix3D& transform, const VertexColorTextureAtlas3D& vec) {
    return { transform * vec.Position, vec.Color, vec.TextureAtlID, vec.TextureCorner };
}