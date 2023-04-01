#pragma once

namespace Game::Blocks {
    using PlayerBlock = Block<BlockType::PLAYER>;
    
    template <> class Block<BlockType::PLAYER> : public BlockBase {
        friend BlockBase;

        protected:
            Block(const Maths::Vec3Int& position, stdu::optional_ref<World> world) : BlockBase(position, world) {}
        public:
            Block() : BlockBase() {}
            Block(const BlockBase& copy) : BlockBase(copy) {}
            Block(BlockBase&& copy) : BlockBase(copy) {}

            ~Block() override = default;

            inline static constexpr BlockType TypeID = BlockType::PLAYER;
            BlockType ID() override { return TypeID; }
    };
}
