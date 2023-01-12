#pragma once
#include <vector>

#include "GraphicsDevice.h"
#include "Mesh.h"
#include "QuadMesh.h"
#include "VertexArray.h"
#include "VertexElement.h"

namespace Graphics
{
    class GraphicsDevice;
    
    /**
     * \brief A class to store meshes in bulk.
     */
    class MeshObject
    {
        using Vertex = VertexColor3D;
        using MeshPtr = std::unique_ptr<Mesh<Vertex>>;
        using MeshPtrRaw = Mesh<Vertex>*;
        std::vector<MeshPtr> _meshes;

        Maths::Matrix3D modelTransform;

        GraphicsDevice* device = nullptr;
        unsigned int deviceIndex = 0;
    public:
        MeshObject() {}
        MeshObject(MeshPtrRaw meshes[], unsigned int meshCount);
        MeshObject(const MeshObject& copy);
        MeshObject(MeshObject&& copy) noexcept;
        MeshObject& operator= (const MeshObject&);
        MeshObject& operator= (MeshObject&&) noexcept;
        
        template<template<class> class TMesh>
        static MeshObject Make(TMesh<Vertex>* meshes[], unsigned int meshCount)
        {
            std::vector<MeshPtrRaw> meshPtrs(meshCount);
            for (unsigned i = 0; i < meshCount; ++i) meshPtrs[i] = meshes[i];
            return MeshObject( meshPtrs.data(), meshCount );
        }
        
        // template<template<class> class TMesh>
        // static void MakeAt(TMesh<Vertex>* meshes[], unsigned int meshCount, MeshObject* out)
        // {
        //     out->_meshes.clear();
        //     out->_meshes.reserve(meshCount);
        //     out->_meshes.insert(out->_meshes.begin(), meshes, meshes + meshCount);
        // }

        ~MeshObject();

        void SetTransform(const Maths::Matrix3D& model);
        void AddTo       (DynamicVertexBuffer<Vertex>& vbuffer, DynamicIndexBuffer& ibuffer) const;
        
        bool IsBound() const { return device; }
        
        void Transform(const Maths::Matrix3D& transform);
        
        void AddMesh(MeshPtrRaw meshes[], unsigned int count);

        void Bind(GraphicsDevice& gdevice);
        void Unbind();

        friend class GraphicsDevice;
    };

    namespace MeshUtils
    {
        using Vec3 = const Maths::Vector3&;
        using Vertex = VertexColor3D;
        MeshObject CubeMesh(Vec3 origin, float x, float y, float z);
        // TODO: MeshObject CubeMesh(Vec3 origin, Vec3 x, Vec3 y, Vec3 z);
    }
}
