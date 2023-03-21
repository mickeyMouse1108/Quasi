#pragma once
#include <array>
#include <optional>

#include "Vector.h"
#include "SerializationUtils.h"
#include "nlohmann/json.hpp"

namespace Game {
    enum class BlockType;
}

namespace Game::Serialization {
    struct BlockStateMatchingStructure {
        template <class T> using option = std::optional<T>;
        option<Maths::Vec3Int> position;
        std::array<option<BlockType>, 6> facing = { };
    };
}

template <>
struct nlohmann::adl_serializer<Game::Serialization::BlockStateMatchingStructure> {
    using BlockMatch = Game::Serialization::BlockStateMatchingStructure;
    static void from_json(const json& js, BlockMatch& bm) {
        get_optional(js, "position", bm.position);
        js.at("facing").get_to(bm.facing);
    }

    static void to_json(json& js, const BlockMatch& bm) {
        js = {
            { "position", bm.position },
            { "sides",    bm.facing }
        };
    }
};