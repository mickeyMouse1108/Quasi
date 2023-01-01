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

BlockMesh::BlockMesh() {
}

BlockMesh::~BlockMesh() {}

Graphics::MeshObject& BlockMesh::GetMeshObjectForm()
{
    std::vector<Graphics::QuadMesh<Graphics::MeshUtils::Vertex>*> faces;
    
    for (int i = 0; i < 6; ++i)
        if (enabledFlags >> i & 1) {
            faces.emplace_back(_blockFaces[faces.size()].get());
        }

    Graphics::MeshObject::MakeAt<Graphics::QuadMesh>(faces.data(), faces.size(), &meshObj);
    return meshObj;
}
