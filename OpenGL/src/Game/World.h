#pragma once

#include "BlockBase.h"
#include "BlockPtr.h"
#include "Serialization/WorldSerialization.h"
#include "stdu/sorted_vector.h"

namespace Game {
    class World {
        // basically std::optional<std::reference_wrapper<T>> but with no overhead 
        template <class T> using opt_ref = const T*;
        
        private:
            Maths::Vec3Int boundsMin, boundsMax;
            static int DefaultBlockComparison(const Maths::Vec3Int& vec) {
                return vec.x * 256 + vec.y * 16 + vec.z;
            }
            static int DefaultBlockComparison(const BlockPtr& x) {
                const auto vec = x->GetPosition();
                return vec.x * 256 + vec.y * 16 + vec.z;
            }
            stdu::sorted_vector<BlockPtr, int(*)(const BlockPtr&)> blocks = { DefaultBlockComparison };

            opt_ref<BlockBase> BlockAt(const Maths::Vec3Int& position, int startIndex = 0);
            
        public:
            World();
            World(const Serialization::WorldStructure& ws);
            ~World();

            void BlockRenderUpdate();
            void Render(Graphics::GraphicsDevice& gd);

            void Load(const std::string& levelname);
            void Build(const Serialization::WorldStructure& structure);
    };
}
    
