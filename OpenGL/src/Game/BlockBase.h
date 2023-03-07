#pragma once
#include <memory>

#include "BlockRenderer.h"
#include "Serialization/BlockSerialization.h"

namespace Game {
    class Block {
        friend BlockRenderer;
        
        private:
        std::unique_ptr<BlockRenderer> Renderer;
        
        Maths::Vec3Int Position = {};
        uint Type = 0;
        public:
        Block(const Maths::Vec3Int& position) : Renderer(new BlockRenderer(*this)), Position(position) {}
        Block(const Serialization::BlockStructure& bs) : Renderer(new BlockRenderer(*this)) { Build(bs); }
        Block(const Block& copy)
            : Renderer(new BlockRenderer(*copy.Renderer)),
              Position(copy.Position),
              Type(copy.Type) {}
        Block(Block&& copy) noexcept
            : Renderer(std::move(copy.Renderer)),
              Position(copy.Position),
              Type(copy.Type) {}
        Block& operator=(const Block& copy) {
            if (this == &copy)
                return *this;
            Renderer = std::unique_ptr { new BlockRenderer(*copy.Renderer) };
            Position = copy.Position;
            Type     = copy.Type;
            return *this;
        }
        Block& operator=(Block&& copy) noexcept {
            if (this == &copy)
                return *this;
            Renderer = std::move(copy.Renderer);
            Position = copy.Position;
            Type     = copy.Type;
            return *this;
        }

        Maths::Vec3Int& GetPosition() { return Position; }
        [[nodiscard]] const Maths::Vec3Int& GetPosition() const { return Position; }
        BlockRenderer& GetRenderer() { return *Renderer; }
        [[nodiscard]] const BlockRenderer& GetRenderer() const { return *Renderer; }

        void Load(const std::string& levelname);
        void Build(const Serialization::BlockStructure& structure);
    };

    enum BlockType {
        NIL = 0,
        WALL,
        PLAYER,
        BOX,
        GOAL
    };

    template <BlockType ID> class Block;
}
