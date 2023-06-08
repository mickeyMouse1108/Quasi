#pragma once
#include "DynamicIndexBuffer.h"
#include "DynamicVertexBuffer.h"
#include "Matrix.h"

namespace Graphics {
    template<class TVertex = Maths::Vector3>
    class Mesh {
        public:
            Mesh() = default;
            virtual ~Mesh() = default;  // NOLINT(clang-diagnostic-deprecated-copy-with-dtor)
        
            virtual void AddTo(DynamicVertexBuffer<TVertex>& vb, DynamicIndexBuffer& ib) = 0;
            virtual void Clone(Mesh& other) const = 0;
            virtual void Clone(std::unique_ptr<Mesh>& other) const = 0;
            
            virtual void Transform(const Maths::Matrix3D& transform) = 0;
    };
}
