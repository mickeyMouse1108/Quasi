#include "Block.h"

#include "Serialization/BlockSerialization.h"

namespace Game {
    void Block::Load(const std::string& levelname) {
        Build(Serialization::BlockStructure::Load(levelname));
    }
    
    void Block::Build(const Serialization::BlockStructure& structure) {
        Position = structure.position;
        Type = structure.type;
    }
}
