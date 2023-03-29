#pragma once
#include "Game/BlockBase.h"

namespace Game::Blocks {
    using WallBlock = Block<BlockType::WALL>;
    
    template <> class Block<BlockType::WALL> : public BlockBase {
        friend BlockBase;

        protected:
            Block(const Maths::Vec3Int& position, stdu::optional_ref<World> world) : BlockBase(position, world) {}
        public:
            Block() : BlockBase() {}
            Block(const BlockBase& copy) : BlockBase(copy) {}
            Block(BlockBase&& copy) : BlockBase(copy) {}

            ~Block() override = default;

            inline static constexpr BlockType TypeID = BlockType::WALL;
            BlockType ID() override { return TypeID; }
    };
}
