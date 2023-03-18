#pragma once
#include "Vector.h"
#include "VectorSerialization.h"
#include "SerializationUtils.h"
#include "nlohmann/json.hpp"

#pragma region TextureAtlasID

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

#pragma endregion

#pragma region BlockTextureStructure

namespace Game::Serialization {
    struct BlockTextureStructure {
        template <class T> using option = std::optional<T>;
        TextureAtlasID all;
        std::array<option<TextureAtlasID>, 6> sides;
    };
}

template <>
struct nlohmann::adl_serializer<Game::Serialization::BlockTextureStructure> {
    using TextureID = Game::Serialization::TextureAtlasID;
    using BlockTexture = Game::Serialization::BlockTextureStructure;
    static void from_json(const json& js, BlockTexture& bs) {
        js.at("all").get_to(bs.all);
        js.at("sides").get_to(bs.sides);
    }

    static void to_json(json& js, const BlockTexture& bs) {
        js = json {
            { "all",   bs.all },
            { "sides", bs.sides }
        };
    }
};

#pragma endregion

#pragma region BlockStateMatchingStructure

// ! to prevent include looping: BlockBase.h -> BlockRenderer.h -> BlockTextureSerialization.h
namespace Game {
    enum class BlockType;
}

namespace Game::Serialization {
    struct BlockStateMatchingStructure {
        template <class T> using option = std::optional<T>;
        option<Maths::Vec3Int> position;
        std::array<std::optional<BlockType>, 6> facing = { };
    };
}

template <>
struct nlohmann::adl_serializer<Game::Serialization::BlockStateMatchingStructure> {
    using BlockMatch = Game::Serialization::BlockStateMatchingStructure;
    static void from_json(const json& js, BlockMatch& bm) {
        js.at("position").get_to(bm.position);
        js.at("facing").get_to(bm.facing);
    }

    static void to_json(json& js, const BlockMatch& bm) {
        js = {
            { "position", bm.position },
            { "sides", bm.facing }
        };
    }
};

#pragma endregion

#pragma region BlockTextureDispatcher

namespace Game::Serialization {
    struct BlockTextureDispatcher {
        BlockType type;
        BlockTextureStructure defaultTexture;
        std::vector<BlockStateMatchingStructure> stateDispatch;
    };
}

template struct nlohmann::adl_serializer<Game::BlockType>;

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

#pragma endregion

#pragma region TextureDispatcher

namespace Game::Serialization {
    struct TextureDispatcher {
        Maths::Vec2Int textureAtlasSize = {};
        std::unordered_map<BlockType, BlockTextureDispatcher> blockTextures;
    };
}

template <>
struct nlohmann::adl_serializer<Game::Serialization::TextureDispatcher> {
    using TextureDisp = Game::Serialization::TextureDispatcher;
    using BlockDisp = Game::Serialization::BlockTextureDispatcher;
    static void from_json(const json& js, TextureDisp& tp) {
        js.at("texSize").get_to(tp.textureAtlasSize);
        for (const auto& btex : js.at("textures")) {
            BlockDisp bp = {};
            btex.get_to<BlockDisp>(bp);
            tp.blockTextures.emplace(bp.type, bp);
        }
    }

    static void to_json(json& js, const TextureDisp& tp) {
        js = json {
            { "texSize",  tp.textureAtlasSize },
            { "textures", {} }
        };
        // funky code
        for (const auto& [ _, bp ] : tp.blockTextures) {
            js.at("textures") += { bp };
        }
    }
};

#pragma endregion
