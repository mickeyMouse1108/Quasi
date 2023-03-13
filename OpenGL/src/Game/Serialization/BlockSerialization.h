#pragma once

#include "Vector.h"
#include "nlohmann/json.hpp"
#include "VectorSerialization.h"

#include <fstream>

#include "Game/BlockBase.h"

namespace Game::Serialization {
    struct BlockStructure {
        Maths::Vec3Int position;
        int type = 0;

        static BlockStructure Load(const std::string& filename);
    };
}

template <>
struct nlohmann::adl_serializer<Game::Serialization::BlockStructure> {
    using BlockStructure = Game::Serialization::BlockStructure;
    static void from_json(const json& js, BlockStructure& bs) {
        js.at("Position").get_to(bs.position);
        js.at("Type").get_to(bs.type);
    }

    static void to_json(json& js, const BlockStructure& bs) {
        js = json { { "Position", bs.position }, { "Type", bs.type } };
    }
};

namespace Game::Serialization {
    inline BlockStructure BlockStructure::Load(const std::string& filename) {
        using nlohmann::json;
        std::ifstream in{ filename };
        json js = json::parse(in);
        return js.get<BlockStructure>();
    }
}
