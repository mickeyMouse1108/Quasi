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

    class MeshObject
    {
        using Vertex = VertexColor3D;
        using MeshPtr = std::unique_ptr<Mesh<Vertex>>;
        std::vector<std::shared_ptr<Mesh<Vertex>>> _meshes;

        Maths::Matrix3D modelTransform;

        GraphicsDevice* device = nullptr;
        unsigned int deviceIndex = 0;
    public:
        MeshObject() {}
        MeshObject(Mesh<Vertex>* meshes[], unsigned int meshCount);
        template<template<class> class TMesh>
                               //WTF?? (passing vector instead of arr because c++ is stupid)
        static MeshObject Make(const std::vector<TMesh<Vertex>*>& meshes)
        {
            std::vector<Mesh<Vertex>*> meshPtrs(meshes.size());
            for (unsigned i = 0; i < meshes.size(); ++i) meshPtrs[i] = meshes[i];
            return MeshObject{ meshPtrs.data(), meshes.size() };
        }

        ~MeshObject();

        void SetTransform(const Maths::Matrix3D& model);
        void AddTo       (DynamicVertexBuffer<Vertex>& vbuffer, DynamicIndexBuffer& ibuffer) const;

        bool IsBound() const { return device; }

        void Transform(const Maths::Matrix3D& transform);

        void AddMesh(Mesh<Vertex>* meshes[], unsigned int count);

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
