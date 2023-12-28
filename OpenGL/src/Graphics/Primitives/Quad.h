#pragma once

#include <array>

#include "GraphicsDevice.h"
#include "Mesh.h"
#include "Vector.h"

namespace Graphics::Primitives {
    class Quad {
        private:
            using Vec3 = Maths::Vector3;
            Vec3 center, x, y;
        public:
            Quad() = default;
            Quad(Vec3 center, Vec3 x, Vec3 y) : center(center), x(x), y(y) {}
            
            OPENGL_API static Quad FromCornerToCorners(Vec3 corner, Vec3 toC1, Vec3 toC2);
            OPENGL_API static Quad FromCenterToCorners(Vec3 center, Vec3 toC1, Vec3 toC2);
                
            OPENGL_API std::array<Vec3, 4> GetVerticies() const;

            OPENGL_API void Transform(const Maths::Matrix3D& transform);
            friend OPENGL_API Quad operator*(const Maths::Matrix3D& transform, const Quad& mesh);

            template <class T> Mesh<T> IntoMesh(Vec3 T::* prop = &T::Position);
    };

    OPENGL_API Quad operator*(const Maths::Matrix3D& transform, const Quad& mesh);

    template <class T>
    Mesh<T> Quad::IntoMesh(Vec3 T::* prop) {
        std::vector<T> vert;
        auto v = GetVerticies();

        vert.resize(4);
        vert[0].*prop = v[0];
        vert[1].*prop = v[1];
        vert[2].*prop = v[2];
        vert[3].*prop = v[3];

        return Mesh<T> { std::move(vert), { { 0, 1, 2 }, { 1, 2, 3 } } };
    }
}
