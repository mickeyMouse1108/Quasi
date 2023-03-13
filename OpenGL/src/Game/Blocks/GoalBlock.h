#pragma once
#include "BoxBlock.h"

namespace Game::Blocks {
    using GoalBlock = Block<GOAL>;
    
    template <> class Block<GOAL> : public BlockBase {
        public:
            Block() : BlockBase() {}
            Block(const Maths::Vec3Int& position) : BlockBase(position) {}
            Block(const Serialization::BlockStructure& bs) : BlockBase(bs) {}
            Block(const BlockBase& copy) : BlockBase(copy) {}
            Block(BlockBase&& copy) : BlockBase(copy) {}

            ~Block() override = default;

            inline static BlockType TypeID = GOAL;
    };
}
