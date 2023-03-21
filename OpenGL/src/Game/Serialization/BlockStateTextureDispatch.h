#pragma once
#include "BlockStateMatching.h"
#include "BlockTextureSerialization.h"

namespace Game {
    enum class BlockType;
}

namespace Game::Serialization {
    struct BlockStateTextureDispatch {
        BlockStateMatchingStructure matchingState;
        BlockTextureStructure dispatchedTexture;
    };
}

template <>
struct nlohmann::adl_serializer<Game::Serialization::BlockStateTextureDispatch> {
    using StateDisp = Game::Serialization::BlockStateTextureDispatch;
    static void from_json(const json& js, StateDisp& sp) {
        js.at("state").get_to(sp.matchingState);
        js.at("texture").get_to(sp.dispatchedTexture);
    }

    static void to_json(json& js, const StateDisp& sp) {
        js = {
            { "state",   sp.matchingState },
            { "texture", sp.dispatchedTexture }
        };
    }
};