#pragma once
#include <vector>
#include <algorithm>

#include "GraphicsDevice.h"
#include "VertexElement.h"
#include "TriIndicies.h"

namespace Graphics {
    /**
     * \brief A class to store meshes in bulk.
     */
    template <class T>
    class Mesh {
        public:
            using Vertex = T;
        private:
            std::vector<Vertex> verticies;
            std::vector<TriIndicies> indicies;

            Maths::Matrix3D modelTransform;

            GraphicsDevice* device = nullptr;
            uint deviceIndex = 0;
        public:
            Mesh() = default;
            Mesh(const std::vector<Vertex>& v, const std::vector<TriIndicies>& i) 
                : verticies(v), indicies(i) {}
            Mesh(std::vector<Vertex>&& v, std::vector<TriIndicies>&& i) 
                : verticies(std::move(v)), indicies(std::move(i)) {}

            ~Mesh() { Unbind(); }

            void SetTransform(const Maths::Matrix3D& model) { modelTransform = model; }
            void Transform(const Maths::Matrix3D& model) { modelTransform *= model; }
            void ApplyTransform(const Maths::Matrix3D& model);
            void AddTo(DynamicVertexBuffer<Vertex>& vbuffer, DynamicIndexBuffer& ibuffer) const;

            // moving isnt that efficient bc vertexes are easy to copy
            void Add(const Mesh& m);

            template <class U> void ApplyMaterial(U T::*prop, U base);
            template <class U> void ApplyMaterial(std::ptrdiff_t prop, U base);

            [[nodiscard]] bool IsBound() const { return device; }

            void Bind(GraphicsDevice& gdevice);
            void Unbind();

            friend class GraphicsDevice;
    };

    template <class T>
    void Mesh<T>::AddTo(DynamicVertexBuffer<Vertex>& vbuffer, DynamicIndexBuffer& ibuffer) const {
        std::vector<T> transformed;
        transformed.resize(verticies.size());
        std::transform(verticies.begin(), verticies.end(), transformed.begin(), [&](auto v) { return modelTransform * v; });
        vbuffer.AddData(transformed);
        ibuffer.AddData((uint*)indicies.data(), indicies.size() * 3, verticies.size() - 1);
    }

    template <class T>
    void Mesh<T>::Add(const Mesh& m) {
        uint off = verticies.size();
        verticies.insert(verticies.end(), m.verticies.begin(), m.verticies.end());
        auto end = indicies.size();
        indicies.insert(indicies.end(), m.indicies.begin(), m.indicies.end());
        std::for_each(indicies.begin() + end, indicies.end(), [=](TriIndicies& i) { i += off; });
    }

    template <class T>
    void Mesh<T>::ApplyTransform(const Maths::Matrix3D& model) {
        for (auto& v : verticies) v *= model;
    }

    template <class T>
    void Mesh<T>::Bind(GraphicsDevice& gdevice) { gdevice.AddMeshes(this, 1); }

    template <class T>
    void Mesh<T>::Unbind() {
        if (device)
            device->Delete((int)deviceIndex);
        device = nullptr;
    }

    template <class T> template <class U>
    void Mesh<T>::ApplyMaterial(U T::* prop, U base) {
        for (auto& v : verticies) {
            v.*prop = base;
        }
    }

    template<class T> template<class U>
    void Mesh<T>::ApplyMaterial(std::ptrdiff_t prop, U base) {
        void* beg = ((void*)verticies[0]) + prop;
        for (int i = 0; i < verticies.size(); ++i, beg += sizeof(T)) {
            *((U*)beg) = base;
        }
    }

    namespace MeshUtils {
        using Vec3 = const Maths::Vector3&;
        OPENGL_API Mesh<> CubeMesh(Vec3 origin, float x, float y, float z);
        // TODO: MeshObject CubeMesh(Vec3 origin, Vec3 x, Vec3 y, Vec3 z);
    }
}
