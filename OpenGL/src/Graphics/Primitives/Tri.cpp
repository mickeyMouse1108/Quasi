#include "Tri.h"

namespace Graphics::Primitives {
    std::array<Maths::fvec3, 3> Tri::GetVertices() const {
        return std::array { a, b, c };
    }

    void Tri::Transform(const Maths::mat3D& transform) {
        *this = transform * *this;
    }

    Tri operator*(const Maths::mat3D& transform, const Tri& mesh) {
        return Tri { transform * mesh.a, transform * mesh.b, transform * mesh.c };
    }
}