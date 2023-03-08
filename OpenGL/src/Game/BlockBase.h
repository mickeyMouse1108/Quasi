#pragma once
#include <memory>

#include "BlockRenderer.h"
#include "Serialization/BlockSerialization.h"

namespace Game {
    // great naming here
    class BlockBase {
        friend BlockRenderer;
        
        private:
        std::unique_ptr<BlockRenderer> Renderer;
        
        Maths::Vec3Int Position = {};
        uint Type = 0;
        public:
        BlockBase(const Maths::Vec3Int& position) : Renderer(new BlockRenderer(*this)), Position(position) {}
        BlockBase(const Serialization::BlockStructure& bs) : Renderer(new BlockRenderer(*this)) { Build(bs); }
        BlockBase(const BlockBase& copy)
            : Renderer(new BlockRenderer(*copy.Renderer)),
              Position(copy.Position),
              Type(copy.Type) {}
        BlockBase(BlockBase&& copy) noexcept
            : Renderer(std::move(copy.Renderer)),
              Position(copy.Position),
              Type(copy.Type) {}
        BlockBase& operator=(const BlockBase& copy) {
            if (this == &copy)
                return *this;
            Renderer = std::unique_ptr { new BlockRenderer(*copy.Renderer) };
            Position = copy.Position;
            Type     = copy.Type;
            return *this;
        }
        BlockBase& operator=(BlockBase&& copy) noexcept {
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

    namespace Blocks {
        template <BlockType ID> class Block;
    }
}
