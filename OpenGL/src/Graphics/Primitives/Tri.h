#pragma once

#include <array>

#include "GraphicsDevice.h"
#include "Mesh.h"
#include "Vector.h"

namespace Graphics::Primitives {
    class Tri {
    private:
        using Vec3 = Maths::Vector3;
        Vec3 a, b, c;
    public:
        Tri() = default;
        Tri(Vec3 a, Vec3 b, Vec3 c) : a(a), b(b), c(c) {}

        OPENGL_API std::array<Vec3, 3> GetVerticies() const;

        OPENGL_API void Transform(const Maths::Matrix3D& transform);
        friend OPENGL_API Tri operator*(const Maths::Matrix3D& transform, const Tri& mesh);

        template <class T> Mesh<T> IntoMesh(Vec3 T::* prop = &T::Position);
    };

    OPENGL_API Tri operator*(const Maths::Matrix3D& transform, const Tri& mesh);

    template <class T>
    Mesh<T> Tri::IntoMesh(Vec3 T::* prop) {
        std::vector<T> vert;
        auto v = GetVerticies();

        vert.resize(3);
        vert[0].*prop = v[0];
        vert[1].*prop = v[1];
        vert[2].*prop = v[2];

        return Mesh<T> { std::move(vert), { { 0, 1, 2 } } };
    }
}