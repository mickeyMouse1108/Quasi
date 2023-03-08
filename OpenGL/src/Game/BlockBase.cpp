#include "BlockBase.h"

#include "Serialization/BlockSerialization.h"

namespace Game {
    void BlockBase::Load(const std::string& levelname) {
        Build(Serialization::BlockStructure::Load(levelname));
    }
    
    void BlockBase::Build(const Serialization::BlockStructure& structure) {
        Position = structure.position;
        Type = structure.type;
    }
}
