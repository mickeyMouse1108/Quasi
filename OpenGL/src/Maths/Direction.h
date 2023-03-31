#pragma once
#include <type_traits>

#include "stdu/enum_utils.h"

namespace Maths {
    enum class Direction2D : int {
        RIGHT = 0,
        LEFT,
        UP,
        DOWN,

        ZERO = 8,
        UNIT = 9
    };
    IMPL_ENUM_OPERATORS(Direction2D)
    
    enum class Direction3D : int {
        RIGHT = 0,
        LEFT,
        UP,
        DOWN,
        FRONT,
        BACK,
    
        ZERO = 8,
        UNIT = 9
    };
    IMPL_ENUM_OPERATORS(Direction3D)

    enum class Direction4D : int {
        RIGHT = 0,
        LEFT,
        UP,
        DOWN,
        FRONT,
        BACK,
        IN,
        OUT,

        ZERO = 8,
        UNIT = 9
    };
    IMPL_ENUM_OPERATORS(Direction4D)

    template <class E>
    concept Direction = requires {
        std::is_enum_v<E>;
        E::ZERO == (E) 8;
        E::UNIT == (E) 9;
    };
}
