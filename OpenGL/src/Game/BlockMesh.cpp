#include "BlockMesh.h"

Graphics::MeshUtils::Vertex BlockMesh::BLOCK_VERTICES[8] = {
    { Maths::Vector3(-0.5f, -0.5f, -0.5f), { 0.0f, 0.0f, 0.0f, 1.0f } }, 
    { Maths::Vector3(-0.5f, -0.5f, +0.5f), { 0.0f, 0.0f, 1.0f, 1.0f } }, 
    { Maths::Vector3(-0.5f, +0.5f, -0.5f), { 0.0f, 1.0f, 0.0f, 1.0f } }, 
    { Maths::Vector3(-0.5f, +0.5f, +0.5f), { 0.0f, 1.0f, 1.0f, 1.0f } }, 
    { Maths::Vector3(+0.5f, -0.5f, -0.5f), { 1.0f, 0.0f, 0.0f, 1.0f } }, 
    { Maths::Vector3(+0.5f, -0.5f, +0.5f), { 1.0f, 0.0f, 1.0f, 1.0f } }, 
    { Maths::Vector3(+0.5f, +0.5f, -0.5f), { 1.0f, 1.0f, 0.0f, 1.0f } }, 
    { Maths::Vector3(+0.5f, +0.5f, +0.5f), { 1.0f, 1.0f, 1.0f, 1.0f } }, 
};

std::unique_ptr<Graphics::QuadMesh<Graphics::MeshUtils::Vertex>> BlockMesh::BLOCK_FACES[6] = {
    std::make_unique<Graphics::QuadMesh<Graphics::MeshUtils::Vertex>>( BLOCK_VERTICES[0], BLOCK_VERTICES[1], BLOCK_VERTICES[2], BLOCK_VERTICES[3] ),
    std::make_unique<Graphics::QuadMesh<Graphics::MeshUtils::Vertex>>( BLOCK_VERTICES[4], BLOCK_VERTICES[5], BLOCK_VERTICES[6], BLOCK_VERTICES[7] ),
    std::make_unique<Graphics::QuadMesh<Graphics::MeshUtils::Vertex>>( BLOCK_VERTICES[0], BLOCK_VERTICES[1], BLOCK_VERTICES[4], BLOCK_VERTICES[5] ),
    std::make_unique<Graphics::QuadMesh<Graphics::MeshUtils::Vertex>>( BLOCK_VERTICES[2], BLOCK_VERTICES[3], BLOCK_VERTICES[6], BLOCK_VERTICES[7] ),
    std::make_unique<Graphics::QuadMesh<Graphics::MeshUtils::Vertex>>( BLOCK_VERTICES[0], BLOCK_VERTICES[2], BLOCK_VERTICES[4], BLOCK_VERTICES[6] ),
    std::make_unique<Graphics::QuadMesh<Graphics::MeshUtils::Vertex>>( BLOCK_VERTICES[1], BLOCK_VERTICES[3], BLOCK_VERTICES[5], BLOCK_VERTICES[7] ),
};

BlockMesh::BlockMesh(const Maths::Vec3Int& position) : position(position) {}

BlockMesh::BlockMesh(const BlockMesh& copy) : enabledFlags(copy.enabledFlags), position(copy.position) {}

BlockMesh::BlockMesh(BlockMesh&& copy) noexcept {
    enabledFlags = copy.enabledFlags;
    position     = copy.position    ;
    meshObj      = std::move(copy.meshObj);
}

BlockMesh::~BlockMesh() {}

Graphics::MeshObject& BlockMesh::GetMeshObjectForm()
{
    std::vector<Graphics::QuadMesh<Graphics::MeshUtils::Vertex>*> faces;
    
    for (int i = 0; i < 6; ++i)
        if (enabledFlags >> i & 1) {
            faces.emplace_back(new Graphics::QuadMesh<Graphics::MeshUtils::Vertex>(*BLOCK_FACES[faces.size()]));
        }

    meshObj = Graphics::MeshObject::Make<Graphics::QuadMesh>(faces.data(), faces.size());
    meshObj.Transform(Maths::Matrix3D::TranslateMat(position.CastF()));
    return meshObj;
}
