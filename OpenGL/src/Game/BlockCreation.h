#pragma once

#include "Blocks/WallBlock.h"
#include "Blocks/PlayerBlock.h"
#include "Blocks/BoxBlock.h"
#include "Blocks/GoalBlock.h"

namespace Game {
    template <BlockType ID>
    std::unique_ptr<BlockBase> BlockBase::Create(const Serialization::BlockStructure& structure)
    requires(BLOCKTYPE_BOUNDMIN <= ID && ID <= BLOCKTYPE_BOUNDMAX) {
        auto block = std::unique_ptr<BlockBase> { (BlockBase*) new Blocks::Block<ID> { Maths::Vec3Int {} } };
        block->Build(structure);
        return block;
    }
    
    template <BlockType CompareID>
    std::unique_ptr<BlockBase> BlockBase::Create(BlockType ID, const Serialization::BlockStructure& structure)
    requires(BLOCKTYPE_BOUNDMIN <= CompareID && CompareID <= BLOCKTYPE_BOUNDMAX) {
        if (CompareID == ID) return Create<CompareID>(structure);
        if constexpr (CompareID + 1 <= BLOCKTYPE_BOUNDMAX) return Create<(BlockType)(CompareID + 1)>(ID, structure);
        else return nullptr;
    }
}
