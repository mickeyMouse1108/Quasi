#include "World.h"

namespace Game {
    World::World() : boundsMin { 0, 0, 0 }, boundsMax { 0, 0, 0 } {}

    World::World(const Serialization::WorldStructure& ws) {
        Build(ws);
    }

    World::~World() {}

    World::opt_ref<BlockBase> World::BlockAt(const Maths::Vec3Int& position, int startIndex) {
        const int comparison = DefaultBlockComparison(position);
        const auto index = blocks.find_predicate( [&](const BlockPtr& x) {
            return comparison - DefaultBlockComparison(x);
        }, startIndex);
        
        return index.exists ? &*blocks[index.index] : nullptr;
    }
    
    void World::BlockRenderUpdate() {
        for (int i = 0; i < blocks.size(); ++i) {
            unsigned cull = 0;
            for (int f = 0; f < 6; ++f) {
                cull |= (BlockAt(blocks[i]->GetPosition() + (Maths::Direction3D)f, i) == nullptr ? 1 : 0) << f;
            }
            blocks[i]->GetRenderer().CullFaces(cull);
        }
    }

    void World::Render(Graphics::GraphicsDevice& gd) {
        for (auto& block : blocks)
            block->GetRenderer().GetMeshObjectForm().Bind(gd);
    }

    void World::Load(const std::string& levelname) {
        Build(Serialization::WorldStructure::Load(levelname));
    }

    void World::Build(const Serialization::WorldStructure& structure) {
        blocks.clear();
        for (const auto& block : structure.tiles) {
            blocks.insert(BlockPtr { BlockBase::Create((BlockType)block.type, block) } );
        }

        blocks.resort();
        BlockRenderUpdate();
        
        boundsMin = structure.boundMin;
        boundsMax = structure.boundMax;
    }
}
