#pragma once
#include "WallBlock.h"

namespace Game::Blocks {
    using PlayerBlock = Block<BlockType::PLAYER>;
    
    template <> class Block<BlockType::PLAYER> : public BlockBase {
        friend BlockBase;

        protected:
            Block(const Maths::Vec3Int& position) : BlockBase(position) {}
        public:
            Block() : BlockBase() {}
            Block(const BlockBase& copy) : BlockBase(copy) {}
            Block(BlockBase&& copy) : BlockBase(copy) {}

            ~Block() override = default;

            inline static constexpr BlockType TypeID = BlockType::PLAYER;
    };
}
