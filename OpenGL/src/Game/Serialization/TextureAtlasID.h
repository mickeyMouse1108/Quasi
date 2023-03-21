#pragma once
#include "Vector.h"
#include "VectorSerialization.h"
#include "nlohmann/json.hpp"

namespace Game::Serialization {
    struct TextureAtlasID {
        inline static Maths::Vec2Int textureAtlSize { 1, 1 };
        int id = -1;

        TextureAtlasID(Maths::Vec2Int idv) : id(idv.x + idv.y * textureAtlSize.x) {}
        TextureAtlasID(int id = 0) : id(id) {}

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
