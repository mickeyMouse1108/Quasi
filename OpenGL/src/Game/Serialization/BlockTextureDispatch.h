#pragma once
#include "BlockStateTextureDispatch.h"
#include "BlockTextureSerialization.h"

namespace Game::Serialization {
    struct BlockTextureDispatcher {
        BlockType type = (BlockType)0;
        BlockTextureStructure defaultTexture {};
        std::vector<BlockStateTextureDispatch> stateDispatch {};
    };
}

template <>
struct nlohmann::adl_serializer<Game::Serialization::BlockTextureDispatcher> {
    using BlockDisp = Game::Serialization::BlockTextureDispatcher;
    static void from_json(const json& js, BlockDisp& bp) {
        js.at("type").get_to(bp.type);
        js.at("default").get_to(bp.defaultTexture);
        js.at("when").get_to(bp.stateDispatch);
    }

    static void to_json(json& js, const BlockDisp& bp) {
        js = json {
                { "type", (int)bp.type },
                { "default",   bp.defaultTexture },
                { "when",      bp.stateDispatch }
        };
    }
};