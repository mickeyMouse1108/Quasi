#pragma once

#include <array>

#include "Vector.h"
#include "MeshObject.h"
#include "Serialization/BlockSerialization.h"

namespace Game
{
    class BlockRenderer
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

        std::array<int, 6> textureID = {0, 1, 2, 3, 4, 5};
    public:
        Maths::Vec3Int position;
        
        BlockRenderer(const Maths::Vec3Int& position = {}, unsigned enabledFlags = BLOCK_FACE_ALL);
        BlockRenderer(const Serialization::BlockStructure& bs);
        
        BlockRenderer(const BlockRenderer& copy);
        BlockRenderer(BlockRenderer&& copy) noexcept;
        BlockRenderer& operator=(const BlockRenderer& copy) noexcept;
        BlockRenderer& operator=(BlockRenderer&& copy) noexcept;
        ~BlockRenderer() = default;

        Maths::Vec3Int& GetPosition() { return position; }
        [[nodiscard]] const Maths::Vec3Int& GetPosition() const { return position; }

        static void SetTexture(Graphics::QuadMesh<Vertex>& mesh, int textureID);

        void CullFaces(unsigned faces) { enabledFlags = faces; }
        void SetTextures(const std::array<int, 6>& tex) { textureID = tex; }

        Graphics::MeshObject& GetMeshObjectForm();

        void Load(const std::string& levelname);
        void Build(const Serialization::BlockStructure& structure);
    };
}
