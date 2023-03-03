#pragma once
#include "BlockSerialization.h"
#include <fstream>

namespace Game::Serialization {
    struct WorldStructure {
        std::vector<BlockStructure> tiles;
        Maths::Vec3Int boundMin, boundMax;
        std::string levelName;

        static WorldStructure Load(const std::string& filename);
    };
}

template <>
struct nlohmann::adl_serializer<Game::Serialization::WorldStructure> {
    static void from_json(const json& js, Game::Serialization::WorldStructure& ws) {
        js.at("Tiles")    .get_to(ws.tiles);
        js.at("BoundMin") .get_to(ws.boundMin);
        js.at("BoundMax") .get_to(ws.boundMax);
        js.at("LevelName").get_to(ws.levelName);
    }

    static void to_json(json& js, const Game::Serialization::WorldStructure& ws) {
        js = json { { "LevelName", ws.levelName },
                    { "BoundMin",  ws.boundMin  },
                    { "BoundMax",  ws.boundMax  },
                    { "Tiles",     ws.tiles     } };
    }
};

namespace Game::Serialization {
    inline WorldStructure WorldStructure::Load(const std::string& filename) {
        using nlohmann::json;
        std::ifstream in{ filename };
        json js = json::parse(in);
        return js.get<WorldStructure>();
    }
}
