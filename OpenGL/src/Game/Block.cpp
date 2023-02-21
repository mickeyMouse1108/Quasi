﻿  #include "Block.h"

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

    constexpr float TEX = 3;
    
    Block::Quad Block::BLOCK_FACES[6] = { // VERT / COL / TEXID / CORNER
        Quad { { BLOCK_VERTICES[0], { 1 }, TEX, 0 }, { BLOCK_VERTICES[1], { 1 }, TEX, 1 },    // D
               { BLOCK_VERTICES[2], { 1 }, TEX, 2 }, { BLOCK_VERTICES[3], { 1 }, TEX, 3 }, },
        Quad { { BLOCK_VERTICES[4], { 1 }, TEX, 0 }, { BLOCK_VERTICES[5], { 1 }, TEX, 1 },    // 
               { BLOCK_VERTICES[6], { 1 }, TEX, 2 }, { BLOCK_VERTICES[7], { 1 }, TEX, 3 }, },
        Quad { { BLOCK_VERTICES[0], { 1 }, TEX, 0 }, { BLOCK_VERTICES[1], { 1 }, TEX, 1 },
               { BLOCK_VERTICES[4], { 1 }, TEX, 2 }, { BLOCK_VERTICES[5], { 1 }, TEX, 3 }, },
        Quad { { BLOCK_VERTICES[2], { 1 }, TEX, 0 }, { BLOCK_VERTICES[3], { 1 }, TEX, 1 },
               { BLOCK_VERTICES[6], { 1 }, TEX, 2 }, { BLOCK_VERTICES[7], { 1 }, TEX, 3 }, },
        Quad { { BLOCK_VERTICES[0], { 1 }, TEX, 0 }, { BLOCK_VERTICES[2], { 1 }, TEX, 1 },
               { BLOCK_VERTICES[4], { 1 }, TEX, 2 }, { BLOCK_VERTICES[6], { 1 }, TEX, 3 }, },
        Quad { { BLOCK_VERTICES[1], { 1 }, TEX, 0 }, { BLOCK_VERTICES[3], { 1 }, TEX, 1 },
               { BLOCK_VERTICES[5], { 1 }, TEX, 2 }, { BLOCK_VERTICES[7], { 1 }, TEX, 3 }, },
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
