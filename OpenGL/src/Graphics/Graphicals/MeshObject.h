#pragma once
#include <vector>

#include "GraphicsDevice.h"
#include "Mesh.h"
#include "QuadMesh.h"
#include "VertexElement.h"

namespace Graphics {
    class GraphicsDevice;
    /**
     * \brief A class to store meshes in bulk.
     */
    class MeshObject {
        public:
            using Vertex = GraphicsDevice::Vertex;
        private:
            using MeshPtr = std::unique_ptr<Mesh<Vertex>>;
            using MeshPtrRaw = Mesh<Vertex>*;
            std::vector<MeshPtr> _meshes;

            Maths::Matrix3D modelTransform;

            GraphicsDevice* device = nullptr;
            uint deviceIndex = 0;
        public:
            MeshObject() = default;
            OPENGL_API MeshObject(MeshPtrRaw meshes[], uint meshCount);
            OPENGL_API MeshObject(const MeshObject& copy);
            OPENGL_API MeshObject(MeshObject&& copy) noexcept;
            OPENGL_API MeshObject& operator= (const MeshObject&);
            OPENGL_API MeshObject& operator= (MeshObject&&) noexcept;
            
            template<template<class> class TMesh>
            static MeshObject Make(const std::vector<std::unique_ptr<TMesh<Vertex>>>& meshes) {
                using TMeshPtr = std::unique_ptr<TMesh<Vertex>>;
                MeshObject mo {};
                mo._meshes.reserve(meshes.size()); 
                std::transform(meshes.begin(), meshes.end(), mo._meshes.begin(),
                    [](const TMeshPtr& ptr) { MeshPtr p = nullptr; ptr->Clone(p); return p; });
                return mo;
            }
            
            template<template<class> class TMesh>
            static MeshObject Make(std::vector<std::unique_ptr<TMesh<Vertex>>>&& meshes) {
                using TMeshPtr = std::unique_ptr<TMesh<Vertex>>;
                MeshObject mo {};
                mo._meshes.reserve(meshes.size()); 
                std::transform(meshes.begin(), meshes.end(), mo._meshes.begin(),
                    [](TMeshPtr& ptr) { return MeshPtr((Mesh<Vertex>*)ptr.release()); });
                return mo;
            }

            template<template<class> class TMesh>
            static MeshObject Make(const std::vector<TMesh<Vertex>*>& meshes) {
                MeshObject mo {};
                mo._meshes = std::vector<MeshPtr> { meshes.size() };
                std::transform(meshes.begin(), meshes.end(), mo._meshes.begin(), [](auto* ptr){ return MeshPtr(ptr); });
                return mo;
            }
            
            // template<template<class> class TMesh>
            // static void MakeAt(TMesh<Vertex>* meshes[], unsigned int meshCount, MeshObject* out)
            // {
            //     out->_meshes.clear();
            //     out->_meshes.reserve(meshCount);
            //     out->_meshes.insert(out->_meshes.begin(), meshes, meshes + meshCount);
            // }

            OPENGL_API ~MeshObject();

            void SetTransform(const Maths::Matrix3D& model) { modelTransform = model; }
            OPENGL_API void AddTo       (DynamicVertexBuffer<Vertex>& vbuffer, DynamicIndexBuffer& ibuffer) const;

            [[nodiscard]] bool IsBound() const { return device; }
            
            OPENGL_API void Transform(const Maths::Matrix3D& transform);
            
            OPENGL_API void AddMesh(MeshPtrRaw meshes[], uint count);

            OPENGL_API void Bind(GraphicsDevice& gdevice);
            OPENGL_API void Unbind();

            friend class GraphicsDevice;
    };

    namespace MeshUtils
    {
        using Vec3 = const Maths::Vector3&;
        using Vertex = MeshObject::Vertex;
        OPENGL_API MeshObject CubeMesh(Vec3 origin, float x, float y, float z);
        // TODO: MeshObject CubeMesh(Vec3 origin, Vec3 x, Vec3 y, Vec3 z);
    }
}
