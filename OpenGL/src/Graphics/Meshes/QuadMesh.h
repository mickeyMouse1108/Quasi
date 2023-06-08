#pragma once

#include <algorithm>
#include <array>

#include "Mesh.h"
#include "Vector.h"

namespace Graphics {
#define MESH_QUAD_CREATE(T, ...) Graphics::QuadMesh<T> { std::array<T, 4> { __VA_ARGS__ } }
    template <class TVertex = Maths::Vector3>
    class QuadMesh final : public Mesh<TVertex> {
        private:
            std::array<TVertex, 4> points;
        public:
            QuadMesh() = default;
            QuadMesh(TVertex _points[4]);
            QuadMesh(const std::array<TVertex, 4>& _points) : points(_points) {}
            QuadMesh(std::array<TVertex, 4>&& _points) : points(std::move(_points)) {}

            std::array<TVertex, 4>& GetVertices() { return points; }
            [[nodiscard]] const std::array<TVertex, 4>& GetVertices() const { return points; }
        
            // friend void AddTo(DynamicVertexBuffer<TVertex>& vb, DynamicIndexBuffer& ib);
            void AddTo(DynamicVertexBuffer<TVertex>& vb, DynamicIndexBuffer& ib) override;

            void Clone(Mesh<TVertex>& other) const override;
            void Clone(std::unique_ptr<Mesh<TVertex>>& other) const override;

            void Transform(const Maths::Matrix3D& transform) override;

            template<typename T = TVertex>
            friend QuadMesh<T> operator*(const Maths::Matrix3D& transform, const QuadMesh<T>& mesh);
    };

    template <class TVertex>
    QuadMesh<TVertex>::QuadMesh(TVertex _points[4]) {
        std::copy(_points, _points + 4, points.begin());
    }

    template<class TVertex>
    void QuadMesh<TVertex>::AddTo(DynamicVertexBuffer<TVertex>& vb, DynamicIndexBuffer& ib) {
        vb.AddData(points.data(), 4);
        const uint i[6] = { 0, 1, 2, 1, 2, 3 };
        ib.AddData(i, 6, -1);
    }

    template <class TVertex>
    void QuadMesh<TVertex>::Clone(Mesh<TVertex>& other) const {
        other = QuadMesh { *this };
    }

    template <class TVertex>
    void QuadMesh<TVertex>::Clone(std::unique_ptr<Mesh<TVertex>>& other) const {
        other = std::unique_ptr<Mesh<TVertex>>( new QuadMesh { *this } );
    }

    template <class TVertex>
    void QuadMesh<TVertex>::Transform(const Maths::Matrix3D& transform) {
        *this = transform * *this;
    }

    template <class TVertex>
    QuadMesh<TVertex> operator*(const Maths::Matrix3D& transform, const QuadMesh<TVertex>& mesh) {
        std::array<TVertex, 4> tp = {};
        std::transform(mesh.points.begin(), mesh.points.end(), tp.begin(),
            [=](const auto& p){ return transform * p; });
        return tp;
    }
}
