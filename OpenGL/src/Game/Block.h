#pragma once

#include "GenericVector.h"
#include "MeshObject.h"

namespace Game
{
    class Block
    {
        using Vertex = Graphics::MeshUtils::Vertex;
        using Quad = Graphics::QuadMesh<Vertex>;
        
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
        static Maths::Vector3 BLOCK_VERTICES[8];
        static Quad BLOCK_FACES[6];
        
        unsigned enabledFlags = BLOCK_FACE_ALL; // 2 ^ 6 - 1 => 111111
        Graphics::MeshObject meshObj;

        Maths::Vec3Int position;
    public:
        Block(const Maths::Vec3Int& position = {}, unsigned enabledFlags = BLOCK_FACE_ALL);
        Block(const Block& copy);
        Block(Block&& copy) noexcept;
        Block& operator=(const Block& copy);
        Block& operator=(Block&& copy) noexcept;
        ~Block();

        Maths::Vec3Int& GetPosition() { return position; }
        [[nodiscard]] const Maths::Vec3Int& GetPosition() const { return position; }

        void CullFaces(unsigned faces) { enabledFlags = faces; }

        Graphics::MeshObject& GetMeshObjectForm();
    };
}
