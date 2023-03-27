#include "BlockBase.h"

#include "World.h"
#include "stdu/optional_ref.h"

namespace Game {
    stdu::optional_ref<BlockBase> BlockBase::BlockInDirection(Maths::Direction3D dir) const {
        return ParentWorld->BlockAt(Position + dir);
    }
}
