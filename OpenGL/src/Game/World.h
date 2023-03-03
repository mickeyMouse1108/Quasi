#pragma once

#include "BlockRenderer.h"
#include "Serialization/WorldSerialization.h"
#include "utils/sorted_vector.h"
#include "utils/func.h"

namespace Game {
    class World {
        // basically std::optional<std::reference_wrapper<T>> but with no overhead 
        template <class T> using opt_ref = const T*;
        
        private:
            Maths::Vec3Int boundsMin, boundsMax;
            static std::function<int(BlockRenderer)> DefaultBlockComparison;
            stdu::sorted_vector<BlockRenderer> blocks {
                std::function { DefaultBlockComparison }
            };

            opt_ref<BlockRenderer> BlockAt(const Maths::Vec3Int& position, int startIndex = 0);
            
        public:
            World();
            World(const Serialization::WorldStructure& ws);
            ~World();

            void BlockUpdate();
            void DisplayTo(Graphics::GraphicsDevice& gd);

            void Load(const std::string& levelname);
            void Build(const Serialization::WorldStructure& structure);
    };
}
    
