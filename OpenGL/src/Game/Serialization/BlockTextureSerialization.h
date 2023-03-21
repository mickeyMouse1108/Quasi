#pragma once
#include "VectorSerialization.h"
#include "TextureAtlasID.h"
#include "SerializationUtils.h"

namespace Game::Serialization {
    struct BlockTextureStructure {
        template <class T> using option = std::optional<T>;
        TextureAtlasID all = 0;
        option<std::array<option<TextureAtlasID>, 6>> sides {};
    };
}

template <>
struct nlohmann::adl_serializer<Game::Serialization::BlockTextureStructure> {
    using TextureID = Game::Serialization::TextureAtlasID;
    using BlockTexture = Game::Serialization::BlockTextureStructure;
    static void from_json(const json& js, BlockTexture& bs) {
        js.at("all").get_to(bs.all);
        get_optional(js, "sides", bs.sides);
    }

    static void to_json(json& js, const BlockTexture& bs) {
        js = json {
            { "all",   bs.all },
            { "sides", bs.sides }
        };
    }
};
