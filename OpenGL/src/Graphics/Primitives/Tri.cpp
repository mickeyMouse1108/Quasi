#include "Tri.h"

namespace Graphics::Primitives {
    std::array<Tri::Vec3, 3> Tri::GetVerticies() const {
        return std::array<Tri::Vec3, 3> { a, b, c };
    }

    void Tri::Transform(const Maths::Matrix3D& transform) {
        *this = transform * *this;
    }

    Tri operator*(const Maths::Matrix3D& transform, const Tri& mesh) {
        return Tri { transform * mesh.a, transform * mesh.b, transform * mesh.c };
    }
}