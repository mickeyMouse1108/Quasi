#pragma once
#include <vector>
#include <algorithm>

#include "RenderData.h"
#include "VertexElement.h"
#include "TriIndicies.h"

namespace Graphics {
    class GenericMesh;
    
    template <class T>
    class Mesh {
    public:
        using Vertex = T;
    private:
        std::vector<Vertex> verticies;
        std::vector<TriIndicies> indicies;

        Maths::Matrix3D modelTransform;

        RenderData* render = nullptr;
        uint deviceIndex = 0;
    public:
        Mesh() = default;
        Mesh(const std::vector<Vertex>& v, const std::vector<TriIndicies>& i)
            : verticies(v), indicies(i) {}
        Mesh(std::vector<Vertex>&& v, std::vector<TriIndicies>&& i)
            : verticies(std::move(v)), indicies(std::move(i)) {}

        ~Mesh() { Unbind(); }

        OPENGL_API static Mesh& Clone(Mesh& dest, const Mesh& from);
        Mesh(const Mesh& mesh) { Clone(*this, mesh); }
        Mesh& operator=(const Mesh& mesh) { return Clone(*this, mesh); }

        OPENGL_API static Mesh& Transfer(Mesh& dest, Mesh&& from);
        Mesh(Mesh&& mesh) noexcept { Transfer(*this, std::move(mesh)); }
        Mesh& operator=(Mesh&& mesh) noexcept { return Transfer(*this, std::move(mesh)); }

        void SetTransform(const Maths::Matrix3D& model) { modelTransform = model; }
        void Transform(const Maths::Matrix3D& model) { modelTransform *= model; }
        void ApplyTransform(const Maths::Matrix3D& model);
        void AddTo(DynamicVertexBuffer& vbuffer, DynamicIndexBuffer& ibuffer) const;

        // moving isnt that efficient bc vertexes are easy to copy
        Mesh& Add(const Mesh& m);
        template <size_t N> static Mesh Combine(std::array<Mesh, N> meshes);

        template <class U> Mesh& ApplyMaterial(U Vertex::* prop, U base);
        template <class U> Mesh& ApplyMaterial(std::ptrdiff_t prop, U base);

        [[nodiscard]] bool IsBound() const { return render; }

        void Bind(RenderData& render);
        void Unbind();

        std::vector<Vertex>& GetVerticies() { return verticies; }
        [[nodiscard]] const std::vector<Vertex>& GetVerticies() const { return verticies; }
        std::vector<TriIndicies>& GetIndicies() { return indicies; }
        [[nodiscard]] const std::vector<TriIndicies>& GetIndicies() const { return indicies; }

        friend class GraphicsDevice;
        friend class RenderData;
        friend class GenericMesh;
    };

    template <class T>
    void Mesh<T>::AddTo(DynamicVertexBuffer& vbuffer, DynamicIndexBuffer& ibuffer) const {
        std::vector<T> transformed;
        transformed.resize(verticies.size());
        std::transform(verticies.begin(), verticies.end(), transformed.begin(), [&](auto v) { return modelTransform * v; });
        vbuffer.AddData(transformed);
        ibuffer.AddData((uint*)indicies.data(), indicies.size() * 3, verticies.size() - 1);
    }

    template <class T>
    Mesh<T>& Mesh<T>::Add(const Mesh<T>& m) {
        uint off = verticies.size();
        verticies.insert(verticies.end(), m.verticies.begin(), m.verticies.end());
        auto end = indicies.size();
        indicies.insert(indicies.end(), m.indicies.begin(), m.indicies.end());
        std::for_each(indicies.begin() + end, indicies.end(), [=](TriIndicies& i) { i += off; });
        return *this;
    }

    template<class T>
    Mesh<T>& Mesh<T>::Clone(Mesh& dest, const Mesh& from) {
        dest.verticies = from.verticies;
        dest.indicies = from.indicies;
        dest.modelTransform = from.modelTransform;
        return dest;
    }

    template<class T>
    Mesh<T>& Mesh<T>::Transfer(Mesh& dest, Mesh&& from) {
        dest.verticies = std::move(from.verticies);
        dest.indicies = std::move(from.indicies);

        dest.modelTransform = from.modelTransform;

        dest.render = from.render;
        from.render = nullptr;

        dest.deviceIndex = from.deviceIndex;
        from.deviceIndex = 0;
        
        if (dest.render)
            dest.render->GetMeshes()[dest.deviceIndex].Set(&dest);

        return dest;
    }

    template <class T>
    void Mesh<T>::ApplyTransform(const Maths::Matrix3D& model) {
        for (auto& v : verticies) v *= model;
    }

    template <class T>
    void Mesh<T>::Bind(RenderData& render) { 
        deviceIndex = render.GetMeshes().size();
        render.GetMeshes().push_back(this);
        this->render = &render;
    }

    template <class T>
    void Mesh<T>::Unbind() {
        if (render)
            render->UnbindMesh((int)deviceIndex);
        render = nullptr;
    }

    template <class T>
    template <size_t N>
    Mesh<T> Mesh<T>::Combine(std::array<Mesh, N> meshes) {
        if constexpr (N == 0) return Mesh<T>();
        Mesh<T> combined = std::move(meshes[0]);
        for (size_t i = 0; i < N; ++i)
            combined.Add(meshes[i]);
        return combined;
    }

    template <class T> template <class U>
    Mesh<T>& Mesh<T>::ApplyMaterial(U Vertex::* prop, U base) {
        for (auto& v : verticies) {
            v.*prop = base;
        }
        return *this;
    }

    template<class T> template<class U>
    Mesh<T>& Mesh<T>::ApplyMaterial(std::ptrdiff_t prop, U base) {
        void* beg = ((void*)verticies[0]) + prop;
        for (int i = 0; i < verticies.size(); ++i, beg += sizeof(T)) {
            *((U*)beg) = base;
        }
        return *this;
    }

    namespace MeshUtils {
        using Vec3 = const Maths::Vector3&;
        OPENGL_API Mesh<VertexColor3D> CubeMesh(Vec3 origin, float x, float y, float z);
        // TODO: MeshObject CubeMesh(Vec3 origin, Vec3 x, Vec3 y, Vec3 z);
    }
}
