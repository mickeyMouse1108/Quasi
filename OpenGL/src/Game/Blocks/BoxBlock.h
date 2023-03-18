#pragma once
#include "PlayerBlock.h"

namespace Game::Blocks {
    using BoxBlock = Block<BOX>;
    
    template <> class Block<BOX> : public BlockBase {
        friend BlockBase;
        
        protected:
            Block(const Maths::Vec3Int& position) : BlockBase(position) {}
        public:
            Block() : BlockBase() {}
            Block(const BlockBase& copy) : BlockBase(copy) {}
            Block(BlockBase&& copy) : BlockBase(copy) {}

            ~Block() override = default;

            inline static constexpr BlockType TypeID = BOX;
    };
}
