#pragma once
#include "Vector.h"
#include "VectorSerialization.h"
#include "nlohmann/json.hpp"

namespace Game::Serialization {
    struct TextureAtlasID {
        static Maths::Vec2Int textureAtlSize;
        int id = -1;

        TextureAtlasID(Maths::Vec2Int idv) : id(idv.x + idv.y * textureAtlSize.x) {}
        TextureAtlasID(int id) : id(id) {}

        operator Maths::Vec2Int() const { return { id / textureAtlSize.x, id % textureAtlSize.y }; }
    };
}

template <>
struct nlohmann::adl_serializer<Game::Serialization::TextureAtlasID> {
    using TextureID = Game::Serialization::TextureAtlasID;
    static void from_json(const json& js, TextureID& bs) {
        if (js.is_array()) bs = { js.get<Maths::Vec2Int>() };
        else bs = { js.get<int>() };
    }

    static void to_json(json& js, const TextureID& bs) {
        js = { (Maths::Vec2Int) bs };
    }
};

namespace Game::Serialization {
    struct BlockTextureStructure {
        TextureAtlasID all;
        std::array<TextureAtlasID, 6> sides = {-1, -1, -1, -1, -1, -1};
    };
}

template <>
struct nlohmann::adl_serializer<Game::Serialization::BlockTextureStructure> {
    using TextureID = Game::Serialization::TextureAtlasID;
    using BlockTexture = Game::Serialization::BlockTextureStructure;
    static void from_json(const json& js, BlockTexture& bs) {
        js.at("all").get_to(bs.all);
        const auto sides = js.at("sides");
        for (int i = 0; i < 6; ++i) {
            sides.at(i).get_to(bs.sides[i]);
        }
    }

    static void to_json(json& js, const BlockTexture& bs) {
        js = json {
            { "all",   bs.all },
            { "sides", bs.sides }
        };
    }
};