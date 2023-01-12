#pragma once

#include "GenericVector.h"
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
    static Graphics::MeshUtils::Vertex BLOCK_VERTICES[8];
    static std::unique_ptr<Graphics::QuadMesh<Graphics::MeshUtils::Vertex>> BLOCK_FACES[6];
    
    unsigned enabledFlags = BLOCK_FACE_ALL; // 2 ^ 6 - 1 => 111111
    Graphics::MeshObject meshObj;

    Maths::Vec3Int position;
public:
    BlockMesh(const Maths::Vec3Int& position = {});
    BlockMesh(const BlockMesh& copy);
    BlockMesh(BlockMesh&& copy) noexcept;
    ~BlockMesh();

    Graphics::MeshObject& GetMeshObjectForm();
};
