#pragma once
#include <algorithm>
#include <array>

#include "Mesh.h"

namespace Graphics {
#define MESH_TRI_CREATE(T, ...) Graphics::TriMesh<T> { std::array<T, 3> { __VA_ARGS__ } }
    template <class TVertex = Maths::Vector3>
    class TriMesh final : public Mesh<TVertex> {
        private:
            std::array<TVertex, 3> points;
        public:
            TriMesh() = default;
            TriMesh(TVertex _points[3]);
            TriMesh(const std::array<TVertex, 3>& _points) : points(_points) {}
            TriMesh(std::array<TVertex, 3>&& _points) : points(std::move(_points)) {}

            std::array<TVertex, 3>& GetVertices() { return points; }
            [[nodiscard]] const std::array<TVertex, 3>& GetVertices() const { return points; }
        
            // friend void AddTo(DynamicVertexBuffer<TVertex>& vb, DynamicIndexBuffer& ib);
            void AddTo(DynamicVertexBuffer<TVertex>& vb, DynamicIndexBuffer& ib) override;
            
            void Clone(Mesh<TVertex>& other) const override;
            void Clone(std::unique_ptr<Mesh<TVertex>>& other) const override;
            
            void Transform(const Maths::Matrix3D& transform) override;

            template<class T = TVertex>
            friend TriMesh<T> operator*(const Maths::Matrix3D& transform, const TriMesh<T>& mesh);
    };

    template <class TVertex>
    TriMesh<TVertex>::TriMesh(TVertex _points[3]) {
        std::copy(_points, _points + 4, points.begin());
    }

    template<class TVertex>
    void TriMesh<TVertex>::AddTo(DynamicVertexBuffer<TVertex>& vb, DynamicIndexBuffer& ib) {
        vb.AddData(points);
        const uint i[3] = { 0, 1, 2 };
        ib.AddData(i, 3, -1);
    }

    template <class TVertex>
    void TriMesh<TVertex>::Clone(Mesh<TVertex>& other) const {
        other = TriMesh { *this };
    }

    template <class TVertex>
    void TriMesh<TVertex>::Clone(std::unique_ptr<Mesh<TVertex>>& other) const {
        other = std::unique_ptr<Mesh<TVertex>>( new TriMesh { *this } );
    }

    template <class TVertex>
    void TriMesh<TVertex>::Transform(const Maths::Matrix3D& transform) {
        *this = transform * *this;
    }

    template <class TVertex>
    TriMesh<TVertex> operator*(const Maths::Matrix3D& transform, const TriMesh<TVertex>& mesh) {
        std::array<TVertex, 3> tp = {};
        std::transform(mesh.points.begin(), mesh.points.end(), tp.begin(),
            [=](const auto& p){ return transform * p; });
        return tp;
    }
}
