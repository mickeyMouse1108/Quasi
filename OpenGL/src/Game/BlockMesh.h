#pragma once

#include "MeshObject.h"

class BlockMesh
{
    static constexpr unsigned BLOCK_FACE_LEFT  = 1 ;
    static constexpr unsigned BLOCK_FACE_RIGHT = 2 ;
    static constexpr unsigned BLOCK_FACE_UP    = 4 ;
    static constexpr unsigned BLOCK_FACE_DOWN  = 8 ;
    static constexpr unsigned BLOCK_FACE_FRONT = 16;
    static constexpr unsigned BLOCK_FACE_BACK  = 32;
    static constexpr unsigned BLOCK_FACE_ALL   = 63;

    
    enum BlockFace
    {
        Left  = 0,
        Right = 1,
        Up    = 2,
        Down  = 3,
        Front = 4,
        Back  = 5,
    };

    //this is really bloated
    std::unique_ptr<Graphics::QuadMesh<Graphics::MeshUtils::Vertex>> _blockFaces[6] = {
        std::make_unique<Graphics::QuadMesh<Graphics::MeshUtils::Vertex>>( BLOCK_VERTICES[0], BLOCK_VERTICES[1], BLOCK_VERTICES[2], BLOCK_VERTICES[3] ),
        std::make_unique<Graphics::QuadMesh<Graphics::MeshUtils::Vertex>>( BLOCK_VERTICES[4], BLOCK_VERTICES[5], BLOCK_VERTICES[6], BLOCK_VERTICES[7] ),
        std::make_unique<Graphics::QuadMesh<Graphics::MeshUtils::Vertex>>( BLOCK_VERTICES[0], BLOCK_VERTICES[1], BLOCK_VERTICES[4], BLOCK_VERTICES[5] ),
        std::make_unique<Graphics::QuadMesh<Graphics::MeshUtils::Vertex>>( BLOCK_VERTICES[2], BLOCK_VERTICES[3], BLOCK_VERTICES[6], BLOCK_VERTICES[7] ),
        std::make_unique<Graphics::QuadMesh<Graphics::MeshUtils::Vertex>>( BLOCK_VERTICES[0], BLOCK_VERTICES[2], BLOCK_VERTICES[4], BLOCK_VERTICES[6] ),
        std::make_unique<Graphics::QuadMesh<Graphics::MeshUtils::Vertex>>( BLOCK_VERTICES[1], BLOCK_VERTICES[3], BLOCK_VERTICES[5], BLOCK_VERTICES[7] ),
    };
    unsigned enabledFlags = BLOCK_FACE_ALL; // 2 ^ 6 - 1 => 111111
    static Graphics::MeshUtils::Vertex BLOCK_VERTICES[8];
    Graphics::MeshObject meshObj;
public:
    BlockMesh();
    ~BlockMesh();

    Graphics::MeshObject& GetMeshObjectForm();
};
