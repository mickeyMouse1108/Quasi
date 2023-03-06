#include "World.h"

namespace Game {
    std::function<int(Block)> World::DefaultBlockComparison = [](const Block& b) {
        const Maths::Vec3Int& vec = b.GetPosition();
        return vec.x * 256 + vec.y * 16 + vec.z;
    };
    
    World::World() : boundsMin { 0, 0, 0 }, boundsMax { 0, 0, 0 } {
        for (int x = 0; x <= 2; ++x)
        for (int y = 0; y <= 2; ++y)
        for (int z = 0; z <= 2; ++z) {
            Block block = { { x, y, z } };
            blocks.insert(block);
        }

        BlockUpdate();
    }

    World::World(const Serialization::WorldStructure& ws) {
        Build(ws);
    }

    World::~World() {}

    World::opt_ref<Block> World::BlockAt(const Maths::Vec3Int& position, int startIndex) {
        const auto index = blocks.find({position}, startIndex);
        return index.exists ? &blocks[index.index] : nullptr;
    }
    
    void World::BlockUpdate() {
        const Maths::Vec3Int DIRECTIONS[6] = {
            Maths::Vec3Int::LEFT ,
            Maths::Vec3Int::RIGHT,
            Maths::Vec3Int::DOWN ,
            Maths::Vec3Int::UP   ,
            Maths::Vec3Int::BACK ,
            Maths::Vec3Int::FRONT,
        };
        
        for (int i = 0; i < blocks.size(); ++i) {
            unsigned cull = 0;
            for (int f = 0; f < 6; ++f) {
                cull |= (BlockAt(blocks[i].GetPosition() + DIRECTIONS[f], i) == nullptr ? 1 : 0) << f;
            }
            blocks[i].GetRenderer().CullFaces(cull);
        }
    }

    void World::Render(Graphics::GraphicsDevice& gd) {
        for (auto& block : blocks)
            block.GetRenderer().GetMeshObjectForm().Bind(gd);
    }

    void World::Load(const std::string& levelname) {
        Build(Serialization::WorldStructure::Load(levelname));
    }

    void World::Build(const Serialization::WorldStructure& structure) {
        blocks.clear();
        blocks.data().resize(structure.tiles.size());
        std::ranges::transform(structure.tiles, blocks.data().begin(), [](auto x){ return Block { x }; });
        
        boundsMin = structure.boundMin;
        boundsMax = structure.boundMax;
    }
}
