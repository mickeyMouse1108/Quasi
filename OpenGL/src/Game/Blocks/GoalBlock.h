#pragma once

namespace Game::Blocks {
    using GoalBlock = Block<BlockType::GOAL>;
    
    template <> class Block<BlockType::GOAL> : public BlockBase {
        friend BlockBase;
        
        protected:
            Block(const Maths::Vec3Int& position, stdu::optional_ref<World> world) : BlockBase(position, world) {}
        public:
            Block() : BlockBase() {}
            Block(const BlockBase& copy) : BlockBase(copy) {}
            Block(BlockBase&& copy) : BlockBase(copy) {}

            ~Block() override = default;

            inline static BlockType TypeID = BlockType::GOAL;
            BlockType ID() override { return TypeID; }
    };
}
