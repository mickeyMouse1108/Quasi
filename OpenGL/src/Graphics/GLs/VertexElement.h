#pragma once
#include "Vector.h"
#include "Matrix.h"

struct VertexColorTexture3D
{
    Maths::Vector3 Position;
    Maths::Vector4 Color;
    Maths::Vector2 TextureCoordinate;
    int TextureID = 0;
};

struct VertexColor3D
{
    Maths::Vector3 Position;
    Maths::Vector4 Color;
};

inline VertexColor3D operator*(const Maths::Matrix3D& transform, const VertexColor3D& vec)
{
    return { transform * vec.Position, vec.Color };
}