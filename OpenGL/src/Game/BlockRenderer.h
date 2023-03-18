#pragma once

#include <array>

#include "Vector.h"
#include "MeshObject.h"
#include "Serialization/BlockTextureSerialization.h"

namespace Game
{
    class BlockBase;
    
    class BlockRenderer
    {
        friend BlockBase;
        
        using Vertex = Graphics::MeshUtils::Vertex;
        using Quad = Graphics::QuadMesh<Vertex>;
        
        static constexpr unsigned BLOCK_FACE_RIGHT = 1 << (int)Maths::Direction3D::RIGHT;
        static constexpr unsigned BLOCK_FACE_LEFT  = 1 << (int)Maths::Direction3D::LEFT ;
        static constexpr unsigned BLOCK_FACE_UP    = 1 << (int)Maths::Direction3D::UP   ;
        static constexpr unsigned BLOCK_FACE_DOWN  = 1 << (int)Maths::Direction3D::DOWN ;
        static constexpr unsigned BLOCK_FACE_FRONT = 1 << (int)Maths::Direction3D::FRONT;
        static constexpr unsigned BLOCK_FACE_BACK  = 1 << (int)Maths::Direction3D::BACK ;
        static constexpr unsigned BLOCK_FACE_ALL   =(1 <<((int)Maths::Direction3D::BACK + 1)) - 1;
        
        static Quad BLOCK_FACES[6];
        
        unsigned enabledFlags = BLOCK_FACE_ALL; // 2 ^ 6 - 1 => 111111
        Graphics::MeshObject meshObj;

        std::array<int, 6> textureID = { 0, 1, 2, 3, 4, 5 };

        BlockBase* parentBlock = nullptr;
        
        BlockRenderer(BlockBase& parent, unsigned enabledFlags = BLOCK_FACE_ALL);
    public:
        BlockRenderer(const BlockRenderer& copy);
        BlockRenderer(BlockRenderer&& copy) noexcept;
        BlockRenderer& operator=(const BlockRenderer& copy) noexcept;
        BlockRenderer& operator=(BlockRenderer&& copy) noexcept;
        ~BlockRenderer() = default;

        [[nodiscard]] const Maths::Vec3Int& GetPosition() const;

        static void SetTextureOfMesh(Graphics::QuadMesh<Vertex>& mesh, int textureID);

        void CullFaces(unsigned faces) { enabledFlags = faces; }
        void SetTextures(Serialization::BlockTextureStructure texture);

        Graphics::MeshObject& GetMeshObjectForm();
    };
}
