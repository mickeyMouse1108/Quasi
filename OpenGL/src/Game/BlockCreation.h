#pragma once

#include "Blocks/WallBlock.h"
#include "Blocks/PlayerBlock.h"
#include "Blocks/BoxBlock.h"
#include "Blocks/GoalBlock.h"

#include "stdu/enum_utils.h"

namespace Game {
    template <BlockType ID>
    std::unique_ptr<BlockBase> BlockBase::Create(const Serialization::BlockStructure& structure)
    requires(BlockType::BOUNDMIN <= ID && ID <= BlockType::BOUNDMAX) {
        auto block = std::unique_ptr<BlockBase> { (BlockBase*) new Blocks::Block<ID> { Maths::Vec3Int {} } };
        block->Build(structure);
        return block;
    }
    
    template <BlockType CompareID>
    std::unique_ptr<BlockBase> BlockBase::Create(BlockType ID, const Serialization::BlockStructure& structure)
    requires(BlockType::BOUNDMIN <= CompareID && CompareID <= BlockType::BOUNDMAX) {
        using namespace stdu;
        if (CompareID == ID) return Create<CompareID>(structure);
        if constexpr (CompareID + 1 <= BlockType::BOUNDMAX) return Create<CompareID + 1>(ID, structure);
        else return nullptr;
    }
}
