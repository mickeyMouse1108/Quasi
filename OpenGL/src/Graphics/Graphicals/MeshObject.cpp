#include "MeshObject.h"

#include "QuadMesh.h"

namespace Graphics
{
    MeshObject::MeshObject(MeshPtrRaw meshes[], unsigned meshCount)
    {
        _meshes.reserve(meshCount);
        for (unsigned i = 0; i < meshCount; ++i) _meshes.emplace_back(meshes[i]);
    }

    MeshObject::MeshObject(const MeshObject& copy) {
        _meshes.reserve(copy._meshes.size());
        for (const auto& mesh : copy._meshes) {
            // create new copy so unique_ptr doesnt delete memory multiple times
            MeshPtr newMesh;
            mesh->Clone(newMesh);
            _meshes.emplace_back(std::move(newMesh));
        }
        modelTransform = copy.modelTransform;
    }

    MeshObject::MeshObject(MeshObject&& copy) noexcept {
        _meshes = std::move(copy._meshes);
        modelTransform = copy.modelTransform;
    }

    MeshObject& MeshObject::operator=(const MeshObject& copy) {
        if (this == &copy) return *this;
        
        _meshes.clear();
        _meshes.reserve(copy._meshes.size());
        for (const auto& mesh : copy._meshes) {
            // create new copy so unique_ptr doesnt delete memory multiple times
            MeshPtr newMesh;
            mesh->Clone(newMesh);
            _meshes.emplace_back(std::move(newMesh));
        }
        modelTransform = copy.modelTransform;
        
        return *this;
    }

    MeshObject& MeshObject::operator=(MeshObject&& copy) noexcept {
        _meshes = std::move(copy._meshes);
        modelTransform = copy.modelTransform;
        return *this;
    }

    MeshObject::~MeshObject()
    {
        Unbind();
    }

    void MeshObject::SetTransform(const Maths::Matrix3D& model)
    {
        modelTransform = model;
    }

    void MeshObject::AddTo(DynamicVertexBuffer<Vertex>& vbuffer, DynamicIndexBuffer& ibuffer) const
    {
        for (unsigned i = 0; i < _meshes.size(); ++i) _meshes[i]->AddTo(vbuffer, ibuffer);
    }
    
    void MeshObject::Transform(const Maths::Matrix3D& transform)
    {
        for (unsigned i = 0; i < _meshes.size(); ++i)
        {
            _meshes[i]->Transform(transform);
        }
    }
    
    void MeshObject::AddMesh(Mesh<Vertex>* meshes[], unsigned count)
    {
        _meshes.insert(_meshes.end(), meshes, meshes + count);
    }

    void MeshObject::Bind(GraphicsDevice& gdevice) { gdevice.AddMeshObject(this, 1); }

    void MeshObject::Unbind()
    {
        if (device)
            device->Delete((int)deviceIndex);
        device = nullptr;
    }

    namespace MeshUtils
    {
        MeshObject CubeMesh(Vec3 origin, float x, float y, float z)
        {
            float hx = x / 2;
            float hy = y / 2;
            float hz = z / 2;
            Vertex vertices[8] =
            {
                { origin + Maths::Vector3(-hx, -hy, -hz), { 0.0f, 0.0f, 0.0f, 1.0f } }, 
                { origin + Maths::Vector3(-hx, -hy, +hz), { 0.0f, 0.0f, 1.0f, 1.0f } }, 
                { origin + Maths::Vector3(-hx, +hy, -hz), { 0.0f, 1.0f, 0.0f, 1.0f } }, 
                { origin + Maths::Vector3(-hx, +hy, +hz), { 0.0f, 1.0f, 1.0f, 1.0f } }, 
                { origin + Maths::Vector3(+hx, -hy, -hz), { 1.0f, 0.0f, 0.0f, 1.0f } }, 
                { origin + Maths::Vector3(+hx, -hy, +hz), { 1.0f, 0.0f, 1.0f, 1.0f } }, 
                { origin + Maths::Vector3(+hx, +hy, -hz), { 1.0f, 1.0f, 0.0f, 1.0f } }, 
                { origin + Maths::Vector3(+hx, +hy, +hz), { 1.0f, 1.0f, 1.0f, 1.0f } }, 
            };

            QuadMesh<Vertex>* cubeMeshes[6] = 
            {
                new MESH_QUAD_CREATE(Vertex, vertices[0], vertices[1], vertices[2], vertices[3]), // left
                new MESH_QUAD_CREATE(Vertex, vertices[4], vertices[5], vertices[6], vertices[7]), // right
                new MESH_QUAD_CREATE(Vertex, vertices[0], vertices[1], vertices[4], vertices[5]), // down
                new MESH_QUAD_CREATE(Vertex, vertices[2], vertices[3], vertices[6], vertices[7]), // up
                new MESH_QUAD_CREATE(Vertex, vertices[0], vertices[2], vertices[4], vertices[6]), // front
                new MESH_QUAD_CREATE(Vertex, vertices[1], vertices[3], vertices[5], vertices[7]), // back
            };
            
            return MeshObject::Make<QuadMesh>(cubeMeshes, 6);
        }
    }
}
