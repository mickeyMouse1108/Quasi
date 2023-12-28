#include "Quad.h"

namespace Graphics::Primitives {
    Quad Quad::FromCornerToCorners(Vec3 corner, Vec3 toC1, Vec3 toC2) {
        Vec3 h1 = toC1 / 2;
        Vec3 h2 = toC2 / 2;
        return Quad(corner + h1 + h2, h1, h2);
    }

    Quad Quad::FromCenterToCorners(Vec3 center, Vec3 toC1, Vec3 toC2) {
        Vec3 h1 = toC1 / 2;
        Vec3 h2 = toC2 / 2;
        return Quad(center, (toC1 + toC2) / 2, (toC2 - toC1) / 2);
    }

    std::array<Quad::Vec3, 4> Quad::GetVerticies() const {
        return std::array<Quad::Vec3, 4> {
            center + x + y,
            center + x - y,
            center - x + y,
            center - x - y
        };
    }

    void Quad::Transform(const Maths::Matrix3D& transform) {
        *this = transform * *this;
    }

    Quad operator*(const Maths::Matrix3D& transform, const Quad& mesh) {
        return Quad { transform * mesh.center, transform * mesh.x, transform * mesh.y };
    }
}