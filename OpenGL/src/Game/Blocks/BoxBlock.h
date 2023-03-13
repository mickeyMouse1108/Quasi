#pragma once
#include "PlayerBlock.h"

namespace Game::Blocks {
    using BoxBlock = Block<BOX>;
    
    template <> class Block<BOX> : public BlockBase {
        public:
            Block() : BlockBase() {}
            Block(const Maths::Vec3Int& position) : BlockBase(position) {}
            Block(const Serialization::BlockStructure& bs) : BlockBase(bs) {}
            Block(const BlockBase& copy) : BlockBase(copy) {}
            Block(BlockBase&& copy) : BlockBase(copy) {}

            ~Block() override = default;

            inline static constexpr BlockType TypeID = BOX;
    };
}
