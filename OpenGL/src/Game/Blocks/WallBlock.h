#pragma once
#include "Game/BlockBase.h"

namespace Game::Blocks {
    using WallBlock = Block<WALL>;
    
    template <> class Block<WALL> : public BlockBase {
        public:
            Block() : BlockBase() {}
            Block(const Maths::Vec3Int& position) : BlockBase(position) {}
            Block(const Serialization::BlockStructure& bs) : BlockBase(bs) {}
            Block(const BlockBase& copy) : BlockBase(copy) {}
            Block(BlockBase&& copy) : BlockBase(copy) {}

            ~Block() override = default;

            inline static constexpr BlockType TypeID = WALL;
    };
}
