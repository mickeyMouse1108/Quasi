#include "World.h"

namespace Game {


    World::World() : minX(-1), minY(-1), maxX(1), maxY(1) {
        for (int x = -1; x <= 1; ++x)
        for (int y = -1; y <= 1; ++y)
        for (int z = -1; z <= 1; ++z) 
                    blocks.insert(Maths::Vec3Int{x, y, z});

        BlockUpdate();
    }
    
    World::~World() {
        
    }

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
            blocks[i].CullFaces(cull);
        }
    }

    void World::DisplayTo(Graphics::GraphicsDevice& gd) {
        for (auto& block : blocks)
            block.GetMeshObjectForm().Bind(gd);
    }
}
