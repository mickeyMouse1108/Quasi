#pragma once
#include <memory>

#include "BlockRenderer.h"
#include "Serialization/BlockSerialization.h"

namespace Game {
    enum BlockType {
        ERR = -1,
        NIL = 0,
        WALL,
        PLAYER,
        BOX,
        GOAL,
        BLOCKTYPE_BOUNDMIN = NIL + 1,
        BLOCKTYPE_BOUNDMAX = GOAL
    };
    
    namespace Blocks {
        template <BlockType ID> class Block {};
    }
    
    // great naming here
    class BlockBase {
        friend BlockRenderer;
        
        private:
            std::unique_ptr<BlockRenderer> Renderer { };
            
            Maths::Vec3Int Position;
        protected:
            BlockBase(const Maths::Vec3Int& position) : Renderer(new BlockRenderer(*this)), Position(position) {}
            BlockBase(const Serialization::BlockStructure& bs) : Renderer(new BlockRenderer(*this)) { BlockBase::Build(bs); }
        public:
            BlockBase() : Renderer(new BlockRenderer(*this)) {}
            BlockBase(const BlockBase& copy)
                : Renderer(new BlockRenderer(*copy.Renderer)),
                  Position(copy.Position) {}
            BlockBase(BlockBase&& copy) noexcept
                : Renderer(std::move(copy.Renderer)),
                  Position(copy.Position) {}
            BlockBase& operator=(const BlockBase& copy) {
                if (this == &copy)
                    return *this;
                Renderer = std::unique_ptr<BlockRenderer> { new BlockRenderer(*copy.Renderer) };
                Position = copy.Position;
                return *this;
            }
            BlockBase& operator=(BlockBase&& copy) noexcept {
                if (this == &copy)
                    return *this;
                Renderer = std::move(copy.Renderer);
                Position = copy.Position;
                return *this;
            }
            
            virtual ~BlockBase() = default;

            Maths::Vec3Int& GetPosition() { return Position; }
            [[nodiscard]] const Maths::Vec3Int& GetPosition() const { return Position; }
            BlockRenderer& GetRenderer() { return *Renderer; }
            [[nodiscard]] const BlockRenderer& GetRenderer() const { return *Renderer; }

            void Load(const std::string& levelname) { Build(Serialization::BlockStructure::Load(levelname)); }
            virtual void Build(const Serialization::BlockStructure& structure) {
                Position = structure.position;
                Renderer->SetTextures({ .all = structure.type });
            }
            
            // * this is probably the most janky code ive ever written,
            template <BlockType ID>
            static std::unique_ptr<BlockBase> Create(const Serialization::BlockStructure& structure)
            requires(BLOCKTYPE_BOUNDMIN <= ID && ID <= BLOCKTYPE_BOUNDMAX);
            // * runtime version of above
            template <BlockType CompareID = BLOCKTYPE_BOUNDMIN>
            static std::unique_ptr<BlockBase> Create(BlockType ID, const Serialization::BlockStructure& structure)
            requires(BLOCKTYPE_BOUNDMIN <= CompareID && CompareID <= BLOCKTYPE_BOUNDMAX);
    };
}

#include "BlockCreation.h"
