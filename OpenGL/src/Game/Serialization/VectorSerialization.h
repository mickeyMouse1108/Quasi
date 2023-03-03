#pragma once
#include <nlohmann/json.hpp>
#include "Vector.h"

#define DECLTYPE_VEC(X) decltype(((X*)nullptr)->x)
#define VEC_LEN(X) (sizeof(X) / sizeof(DECLTYPE_VEC(X)))

// template <class T>
// struct nlohmann::adl_serializer<Maths::Vec3<T>> {
//     static void from_json(const json& json, Maths::Vec3<T>& vec) {
//         if (json.is_array()) {
//             vec = { (T)json[ 0 ], (T)json[ 1 ], (T)json[ 2 ] };
//             return;
//         }
//         vec = { (T)json["x"], (T)json["y"], (T)json["z"] };
//     }
//
//     // case 2 + IS_ARRAY: vec = TVec { (TElem)json[ 0 ], (TElem)json[ 1 ] };                                     return;
//     // case 3 + IS_ARRAY: vec = TVec { (TElem)json[ 0 ], (TElem)json[ 1 ], (TElem)json[ 2 ] };                   return;
//     // case 4 + IS_ARRAY: vec = TVec { (TElem)json[ 0 ], (TElem)json[ 1 ], (TElem)json[ 2 ], (TElem)json[ 3 ] }; return;
//     // case 2 + IS_DICT : vec = TVec { (TElem)json["x"], (TElem)json["y"] };                                     return;
//     // case 3 + IS_DICT : vec = TVec { (TElem)json["x"], (TElem)json["y"], (TElem)json["z"] };                   return;
//     // case 4 + IS_DICT : vec = TVec { (TElem)json["x"], (TElem)json["y"], (TElem)json["z"], (TElem)json["w"] }; return;
//     // default:;
//
//     static void to_json(json& json, Maths::Vec3<T> vec) {
//         json = json::array({ vec.x, vec.y, vec.z });
//     }
// };

template <Maths::Vec T>
struct nlohmann::adl_serializer<T> {
    static void from_json(const json& json, T& vec) {
        constexpr int VEC_SIZE = VEC_LEN(T);
        using TElem = DECLTYPE_VEC(T);
        if (json.is_array()) {
            if      constexpr (VEC_SIZE == 2) vec = T { (TElem)json[0], (TElem)json[1] };
            else if constexpr (VEC_SIZE == 3) vec = T { (TElem)json[0], (TElem)json[1], (TElem)json[2] };
            else if constexpr (VEC_SIZE == 4) vec = T { (TElem)json[0], (TElem)json[1], (TElem)json[2], (TElem)json[3]  };
            return;
        }
        if      constexpr (VEC_SIZE == 2) vec = T { (TElem)json["x"], (TElem)json["y"] };
        else if constexpr (VEC_SIZE == 3) vec = T { (TElem)json["x"], (TElem)json["y"], (TElem)json["z"] };
        else if constexpr (VEC_SIZE == 4) vec = T { (TElem)json["x"], (TElem)json["y"], (TElem)json["z"], (TElem)json["w"]  };
    }

    static void to_json(json& json, const T& vec) {
        constexpr int VEC_SIZE = VEC_LEN(T);
        if      constexpr (VEC_SIZE == 2) json = json::array({ vec.x, vec.y});
        else if constexpr (VEC_SIZE == 3) json = json::array({ vec.x, vec.y, vec.z });
        else if constexpr (VEC_SIZE == 4) json = json::array({ vec.x, vec.y, vec.z, vec.w });
    }
};

#undef VEC_LEN
#undef DECLTYPE_VEC
