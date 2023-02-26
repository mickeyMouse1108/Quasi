#pragma once
#include "NumTypes.h"

namespace Maths
{
    template <class TVec>
    concept Vec = requires (TVec vec) {
        sizeof(TVec) % sizeof(decltype(vec.x)) == 0;
        sizeof(TVec) / sizeof(decltype(vec.x)) > 1;
    };
    
    template <class TBase, class TVec>
    concept VecLessThan = requires () {
        Vec<TVec>;
        sizeof(TVec) < sizeof(TBase);
    };

    template <class TBase, class TVec>
    concept VecGreaterThan = requires () {
        Vec<TVec>;
        sizeof(TVec) > sizeof(TBase);
    };
    
    template <class T>
    concept Numeral = requires (T self, T other) {
        { self + other } -> std::convertible_to<T>;
        { self - other } -> std::convertible_to<T>;
        { self * other } -> std::convertible_to<T>;
        { self / other } -> std::convertible_to<T>;
        (T) 0;
        (T) 1;
        (T)-1;
    };

    template <Numeral T> struct Vec2;
    template <Numeral T> struct Vec3;
    template <Numeral T> struct Vec4;
    using Vector2 = Vec2<float>;
    using Vector3 = Vec3<float>;
    using Vector4 = Vec4<float>;
    using Vec2Int = Vec2<int>;
    using Vec3Int = Vec3<int>;
    using Vec4Int = Vec4<int>;
    using Vec2UInt = Vec2<uint>;
    using Vec3UInt = Vec3<uint>;
    using Vec4UInt = Vec4<uint>;

    // TODO: Add non-numeral Vecs
    // TODO: Add math operations like: lerp, length

#pragma region Vec2 Declaration
    template <Numeral T> struct Vec2
    {
        static const Vec2 RIGHT;
        static const Vec2 LEFT ;
        static const Vec2 UP   ;
        static const Vec2 DOWN ;
        static const Vec2 ZERO ;
        
        T x, y;
        Vec2(T x = 0, T y = 0) : x(x), y(y) {}
        Vec2(T val) : x(val), y(val) {}
        Vec2(const Vec3<T>& vec);
        Vec2(const Vec4<T>& vec);

        T operator[] (unsigned int i) const { return ((const T*)this)[i]; }
        operator T*() { return (T*)this; }

#define DEF_VEC2_OP(OP)     Vec2 operator OP (const Vec2& other) const { return { x OP other.x, y OP other.y }; } \
                            Vec2 operator OP (T&& other) const { return { x OP other, y OP other }; } \
                            Vec2 operator OP (const T& other) const { return { x OP other, y OP other }; }

        DEF_VEC2_OP(+)
        DEF_VEC2_OP(-)
        DEF_VEC2_OP(*)
        DEF_VEC2_OP(/)
        DEF_VEC2_OP(%)

#undef DEF_VEC2_OP
#define DEF_VEC2_CAST(TYPE)
        template <Numeral TOther> operator Vec2<TOther>() { return { (TOther)x, (TOther)y }; }
        template <VecLessThan<Vec2>    TVec> operator TVec() { return TVec { *this }; }
        template <VecGreaterThan<Vec2> TVec> operator TVec() { return TVec { x, y }; }
    };
#pragma endregion 

#pragma region Vec3 Declaration
    template <Numeral T> struct Vec3
    {
        static const Vec3 RIGHT;
        static const Vec3 LEFT ;
        static const Vec3 UP   ;
        static const Vec3 DOWN ;
        static const Vec3 FRONT;
        static const Vec3 BACK ;
        static const Vec3 ZERO ;
        
        T x, y, z;
        Vec3(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z) {}
        Vec3(T val) : x(val), y(val), z(val) {}
        Vec3(const Vec4<T>& vec);

        T operator[] (unsigned int i) const { return ((const T*)this)[i]; }
        operator T*() { return (T*)this; }

#define DEF_VEC3_OP(OP) Vec3 operator OP (const Vec3& other) const { return { x OP other.x, y OP other.y, z OP other.z }; } \
                        Vec3 operator OP (T&& other) const { return { x OP other, y OP other, z OP other }; } \
                        Vec3 operator OP (const T& other) const { return { x OP other, y OP other, z OP other }; }

        DEF_VEC3_OP(+)
        DEF_VEC3_OP(-)
        DEF_VEC3_OP(*)
        DEF_VEC3_OP(/)
        DEF_VEC3_OP(%)

#undef DEF_VEC3_OP
        template <Numeral TOther> operator Vec3<TOther>() { return { (TOther)x, (TOther)y, (TOther)z }; }
        template <VecLessThan<Vec3>    TVec> operator TVec() { return TVec { *this }; }
        template <VecGreaterThan<Vec3> TVec> operator TVec() { return TVec { x, y, z }; }
    };
#pragma endregion

#pragma region Vec4 Declaration
    template <Numeral T> struct Vec4
    {
        static const Vec4 RIGHT;
        static const Vec4 LEFT ;
        static const Vec4 UP   ;
        static const Vec4 DOWN ;
        static const Vec4 FRONT;
        static const Vec4 BACK ;
        static const Vec4 IN   ;
        static const Vec4 OUT  ;
        static const Vec4 ZERO ;
        
        T x, y, z, w;
        Vec4(T x = 0, T y = 0, T z = 0, T w = 0) : x(x), y(y), z(z), w(w) {}
        Vec4(T val) : x(val), y(val), z(val), w(val) {}

        T operator[] (unsigned int i) const { return ((const T*)this)[i]; }
        operator T*() { return (T*)this; }

#define DEF_VEC4_OP(OP) Vec4 operator OP (const Vec4& other) const { return { x OP other.x, y OP other.y, z OP other.z, w OP other.w }; } \
                        Vec4 operator OP (T&& other) const { return { x OP other, y OP other, z OP other, w OP other }; } \
                        Vec4 operator OP (const T& other) const { return { x OP other, y OP other, z OP other, w OP other }; }

        DEF_VEC4_OP(+)
        DEF_VEC4_OP(-)
        DEF_VEC4_OP(*)
        DEF_VEC4_OP(/)
        DEF_VEC4_OP(%)

#undef DEF_VEC3_OP
        template <Numeral TOther> operator Vec4<TOther>() { return { (TOther)x, (TOther)y, (TOther)z, (TOther)w }; }
        template <VecLessThan<Vec4>    TVec> operator TVec() { return TVec { *this }; }
        template <VecGreaterThan<Vec4> TVec> operator TVec() { return TVec { x, y, z, w }; }
    };
#pragma endregion

#pragma region Vec2 Definition
    template <Numeral T> const Vec2<T> Vec2<T>::RIGHT = {  1,  0 };
    template <Numeral T> const Vec2<T> Vec2<T>::LEFT  = { -1,  0 };
    template <Numeral T> const Vec2<T> Vec2<T>::UP    = {  0,  1 };
    template <Numeral T> const Vec2<T> Vec2<T>::DOWN  = {  0, -1 };
    template <Numeral T> const Vec2<T> Vec2<T>::ZERO  = {  0,  0 };

    template <Numeral T> Vec2<T>::Vec2(const Vec3<T>& vec) : x(vec.x), y(vec.y) {}
    template <Numeral T> Vec2<T>::Vec2(const Vec4<T>& vec) : x(vec.x), y(vec.y) {}
#pragma endregion

#pragma region Vec3 Definition
    template <Numeral T> const Vec3<T> Vec3<T>::RIGHT = {  1,  0,  0 };
    template <Numeral T> const Vec3<T> Vec3<T>::LEFT  = { -1,  0,  0 };
    template <Numeral T> const Vec3<T> Vec3<T>::UP    = {  0,  1,  0 };
    template <Numeral T> const Vec3<T> Vec3<T>::DOWN  = {  0, -1,  0 };
    template <Numeral T> const Vec3<T> Vec3<T>::FRONT = {  0,  0,  1 };
    template <Numeral T> const Vec3<T> Vec3<T>::BACK  = {  0,  0, -1 };
    template <Numeral T> const Vec3<T> Vec3<T>::ZERO  = {  0,  0,  0 };

    template <Numeral T> Vec3<T>::Vec3(const Vec4<T>& vec) : x(vec.x), y(vec.y), z(vec.z) {}
#pragma endregion

#pragma region Vec4 Definition
    template <Numeral T> const Vec4<T> Vec4<T>::RIGHT = {  1,  0,  0,  0 };
    template <Numeral T> const Vec4<T> Vec4<T>::LEFT  = { -1,  0,  0,  0 };
    template <Numeral T> const Vec4<T> Vec4<T>::UP    = {  0,  1,  0,  0 };
    template <Numeral T> const Vec4<T> Vec4<T>::DOWN  = {  0, -1,  0,  0 };
    template <Numeral T> const Vec4<T> Vec4<T>::FRONT = {  0,  0,  1,  0 };
    template <Numeral T> const Vec4<T> Vec4<T>::BACK  = {  0,  0, -1,  0 };
    template <Numeral T> const Vec4<T> Vec4<T>::IN    = {  0,  0,  0,  1 };
    template <Numeral T> const Vec4<T> Vec4<T>::OUT   = {  0,  0,  0, -1 };
    template <Numeral T> const Vec4<T> Vec4<T>::ZERO  = {  0,  0,  0,  0 };
#pragma endregion
}
