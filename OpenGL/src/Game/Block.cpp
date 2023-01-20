#include "Block.h"

namespace Game
{
    
    Maths::Vector3 Block::BLOCK_VERTICES[8] = {
        { -0.5f, -0.5f, -0.5f }, 
        { -0.5f, -0.5f, +0.5f }, 
        { -0.5f, +0.5f, -0.5f }, 
        { -0.5f, +0.5f, +0.5f }, 
        { +0.5f, -0.5f, -0.5f }, 
        { +0.5f, -0.5f, +0.5f }, 
        { +0.5f, +0.5f, -0.5f }, 
        { +0.5f, +0.5f, +0.5f }, 
    };

    Block::Quad Block::BLOCK_FACES[6] = {
        Quad { { BLOCK_VERTICES[0], { 1, 0, 0, 1 } }, { BLOCK_VERTICES[1], { 1, 0, 0, 1 } },    // D
               { BLOCK_VERTICES[2], { 1, 0, 0, 1 } }, { BLOCK_VERTICES[3], { 1, 0, 0, 1 } }, },
        Quad { { BLOCK_VERTICES[4], { 0, 1, 1, 1 } }, { BLOCK_VERTICES[5], { 0, 1, 1, 1 } },    // 
               { BLOCK_VERTICES[6], { 0, 1, 1, 1 } }, { BLOCK_VERTICES[7], { 0, 1, 1, 1 } }, },
        Quad { { BLOCK_VERTICES[0], { 0, 1, 0, 1 } }, { BLOCK_VERTICES[1], { 0, 1, 0, 1 } },
               { BLOCK_VERTICES[4], { 0, 1, 0, 1 } }, { BLOCK_VERTICES[5], { 0, 1, 0, 1 } }, },
        Quad { { BLOCK_VERTICES[2], { 1, 0, 1, 1 } }, { BLOCK_VERTICES[3], { 1, 0, 1, 1 } },
               { BLOCK_VERTICES[6], { 1, 0, 1, 1 } }, { BLOCK_VERTICES[7], { 1, 0, 1, 1 } }, },
        Quad { { BLOCK_VERTICES[0], { 0, 0, 1, 1 } }, { BLOCK_VERTICES[2], { 0, 0, 1, 1 } },
               { BLOCK_VERTICES[4], { 0, 0, 1, 1 } }, { BLOCK_VERTICES[6], { 0, 0, 1, 1 } }, },
        Quad { { BLOCK_VERTICES[1], { 1, 1, 0, 1 } }, { BLOCK_VERTICES[3], { 1, 1, 0, 1 } },
               { BLOCK_VERTICES[5], { 1, 1, 0, 1 } }, { BLOCK_VERTICES[7], { 1, 1, 0, 1 } }, },
    };

    Block::Block(const Maths::Vec3Int& position, unsigned enabledFlags) : enabledFlags(enabledFlags), position(position) {}

    Block::Block(const Block& copy) : enabledFlags(copy.enabledFlags), position(copy.position) {}

    Block::Block(Block&& copy) noexcept {
        enabledFlags = copy.enabledFlags;
        position     = copy.position    ;
    }

    Block& Block::operator=(const Block& copy) {
        enabledFlags = copy.enabledFlags;
        position = copy.position;
        return *this;
    }

    Block& Block::operator=(Block&& copy) noexcept {
        enabledFlags = copy.enabledFlags;
        position     = copy.position    ;
        return *this;
    }

    Block::~Block() {}

    Graphics::MeshObject& Block::GetMeshObjectForm()
    {
        std::vector<Quad*> faces;
        
        for (int i = 0; i < 6; ++i)
            if (enabledFlags & 1 << i) {
                faces.emplace_back(new Graphics::QuadMesh(BLOCK_FACES[i]));
            }

        meshObj = Graphics::MeshObject::Make<Graphics::QuadMesh>(faces.data(), faces.size());
        meshObj.Transform(Maths::Matrix3D::TranslateMat(position.CastF()));
        return meshObj;
    }
}
