#pragma once
#include "BoxBlock.h"

namespace Game::Blocks {
    using GoalBlock = Block<GOAL>;
    
    template <> class Block<GOAL> : public BlockBase {
        friend BlockBase;
        
        protected:
            Block(const Maths::Vec3Int& position) : BlockBase(position) {}
        public:
            Block() : BlockBase() {}
            Block(const BlockBase& copy) : BlockBase(copy) {}
            Block(BlockBase&& copy) : BlockBase(copy) {}

            ~Block() override = default;

            inline static BlockType TypeID = GOAL;
    };
}
