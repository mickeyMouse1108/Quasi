#pragma once
#include "DynamicIndexBuffer.h"
#include "DynamicVertexBuffer.h"
#include "Matrix.h"

namespace Graphics
{
    template<typename TVertex = Maths::Vector3>
    class Mesh
    {
    public:
        Mesh() = default;
        virtual ~Mesh() = default;

        virtual void GetData(unsigned int* vertexSize, TVertex** vertices, unsigned int* indexSize, unsigned int** indices) = 0;
        
        virtual void AddTo(DynamicVertexBuffer<TVertex>& vb, DynamicIndexBuffer& ib) = 0;
        
        virtual void Transform(const Maths::Matrix3D& transform) = 0;
    };
}
