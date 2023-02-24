#pragma once

#include "BlockRenderer.h"
#include "utils/sorted_vector.h"

namespace Game {
    class World {
        // basically std::optional<std::reference_wrapper<T>> but with no overhead 
        template <class T> using opt_ref = const T*;
        
        private:
        int minX, minY, maxX, maxY;
        stdu::sorted_vector<BlockRenderer> blocks {
            [](const BlockRenderer& b) {
                const Maths::Vec3Int& vec = b.GetPosition();
                return vec.x * 256 + vec.y * 16 + vec.z;
            }
        };

        opt_ref<BlockRenderer> BlockAt(const Maths::Vec3Int& position, int startIndex = 0);
        
        public:
        World();
        ~World();

        void BlockUpdate();
        void DisplayTo(Graphics::GraphicsDevice& gd);
    };
}
    
