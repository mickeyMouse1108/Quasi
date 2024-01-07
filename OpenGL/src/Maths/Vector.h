#pragma once
#include "NumTypes.h"

#include "Corner.h"
#include "Direction.h"

namespace Maths {
    template <class T> struct vec2;
    template <class T> struct vec3;
    template <class T> struct vec4;

    template <int, class> struct vecn { using type = void; };
    template <class T> struct vecn<2, T> { using type = vec2<T>; };
    template <class T> struct vecn<3, T> { using type = vec3<T>; };
    template <class T> struct vecn<4, T> { using type = vec4<T>; };
    
    template <class>
    struct Vec : std::false_type {};
    
    template <class TElement>
    struct Vec<vec2<TElement>> : std::true_type {};
    template <class TElement>
    struct Vec<vec3<TElement>> : std::true_type {};
    template <class TElement>
    struct Vec<vec4<TElement>> : std::true_type {};

    template <class V> constexpr bool IsVecV = Vec<V>::value;
    
    template <class TBase, class TVec>
    concept VecLessThan = requires () {
        IsVecV<TVec>;
        sizeof(TVec) < sizeof(TBase);
    };

    template <class TBase, class TVec>
    concept VecGreaterThan = requires () {
        IsVecV<TVec>;
        sizeof(TVec) > sizeof(TBase);
    };

    using fvec2 = vec2<float>;
    using fvec3 = vec3<float>;
    using fvec4 = vec4<float>;
    using dvec2 = vec2<double>;
    using dvec3 = vec3<double>;
    using dvec4 = vec4<double>;
    using ivec2 = vec2<int>;
    using ivec3 = vec3<int>;
    using ivec4 = vec4<int>;
    using uvec2 = vec2<uint>;
    using uvec3 = vec3<uint>;
    using uvec4 = vec4<uint>;

    // TODO: Add non-numeral Vecs
    // TODO: Add math operations like: lerp, length

#pragma region Vec2 Declaration
    template <class T> struct vec2 {
        static const vec2 RIGHT;
        static const vec2 LEFT ;
        static const vec2 UP   ;
        static const vec2 DOWN ;
        
        static const vec2 ZERO ;
        static const vec2 ONE  ;
        
        T x, y;
        vec2(T s = 0) : x(s), y(s) {}
        vec2(T x, T y) : x(x), y(y) {}
        vec2(Direction2D dir, T scale = 1);
        vec2(Corner2D    cor, T scale = 1);
        vec2(const vec3<T>& xy);
        vec2(const vec4<T>& xy);

        T operator[] (unsigned int i) const { return ((const T*)this)[i]; }
        operator T*() { return (T*)this; }

#define DEF_VEC2_OP(OP) vec2 operator OP (const vec2& other) const { return { x OP other.x, y OP other.y }; } \
                        vec2 operator OP (T other) const { return { x OP other, y OP other }; } \
                        vec2& operator OP##= (const vec2& other) { x OP##= other.x; y OP##= other.y; return *this; } \
                        vec2& operator OP##= (T other) { x OP##= other; y OP##= other; return *this; }

        DEF_VEC2_OP(+)
        DEF_VEC2_OP(-)
        DEF_VEC2_OP(*)
        DEF_VEC2_OP(/)
        DEF_VEC2_OP(%)

        vec2 operator+() const { return *this; }
        vec2 operator-() const { return { -x, -y }; }
        
        bool operator==(const vec2& other) const { return x == other.x && y == other.y; }
        bool operator!=(const vec2& other) const { return !(*this == other); }

#undef DEF_VEC2_OP
#define DEF_VEC2_CAST(TYPE)
        template <class TOther> operator vec2<TOther>() { return { (TOther)x, (TOther)y }; }
        template <VecLessThan<vec2>    TVec> operator TVec() { return TVec { *this }; }
        template <VecGreaterThan<vec2> TVec> operator TVec() { return TVec { x, y }; }
    };
#pragma endregion 

#pragma region Vec3 Declaration
    template <class T> struct vec3 {
        static const vec3 RIGHT;
        static const vec3 LEFT ;
        static const vec3 UP   ;
        static const vec3 DOWN ;
        static const vec3 FRONT;
        static const vec3 BACK ;
        
        static const vec3 ZERO ;
        static const vec3 ONE  ;
        
        T x, y, z;
        vec3(T s = 0) : x(s), y(s), z(s) {}
        vec3(T x, T y, T z) : x(x), y(y), z(z) {}
        vec3(Direction3D dir, T scale = 1);
        vec3(Corner3D    cor, T scale = 1);
        vec3(const vec2<T>& xy, float z);
        vec3(const vec4<T>& xyz);

        T operator[] (unsigned int i) const { return ((const T*)this)[i]; }
        operator T*() { return (T*)this; }

#define DEF_VEC3_OP(OP) vec3 operator OP (const vec3& other) const { return { x OP other.x, y OP other.y, z OP other.z }; } \
                        vec3 operator OP (T other) const { return { x OP other, y OP other, z OP other }; } \
                        vec3& operator OP##= (const vec3& other) { x OP##= other.x; y OP##= other.y; z OP##= other.z; return *this; } \
                        vec3& operator OP##= (T other) { x OP##= other; y OP##= other; z OP##= other; return *this; }
        
        DEF_VEC3_OP(+)
        DEF_VEC3_OP(-)
        DEF_VEC3_OP(*)
        DEF_VEC3_OP(/)
        DEF_VEC3_OP(%)

        vec3 operator+() const { return *this; }
        vec3 operator-() const { return { -x, -y, -z }; }

        bool operator==(const vec3& other) const { return x == other.x && y == other.y && z == other.z; }
        bool operator!=(const vec3& other) const { return !(*this == other); }

#undef DEF_VEC3_OP
        template <class TOther> operator vec3<TOther>() { return { (TOther)x, (TOther)y, (TOther)z }; }
        template <VecLessThan<vec3>    TVec> operator TVec() { return TVec { *this }; }
        template <VecGreaterThan<vec3> TVec> operator TVec() { return TVec { x, y, z }; }
    };
#pragma endregion

#pragma region Vec4 Declaration
    template <class T> struct vec4 {
        static const vec4 RIGHT;
        static const vec4 LEFT ;
        static const vec4 UP   ;
        static const vec4 DOWN ;
        static const vec4 FRONT;
        static const vec4 BACK ;
        static const vec4 IN   ;
        static const vec4 OUT  ;
        
        static const vec4 ZERO ;
        static const vec4 ONE  ;
        
        T x, y, z, w;
        vec4() : x(0), y(0), z(0), w(0) {}
        vec4(T s, T w = 1) : x(s), y(s), z(s), w(w) {} // special w = 1 by default
        vec4(T x, T y, T z, T w = 1) : x(x), y(y), z(z), w(w) {} // special w = 1 by default
        vec4(Direction4D dir, T scale = 1);
        vec4(Corner4D    cor, T scale = 1);
        vec4(const vec2<T>& xy, float z, float w = 1);
        vec4(const vec2<T>& xy, const vec2<T>& zw);
        vec4(const vec3<T>& xyz, float w = 1);

        T operator[] (unsigned int i) const { return ((const T*)this)[i]; }
        operator T*() { return (T*)this; }

#define DEF_VEC4_OP(OP) vec4 operator OP (const vec4& other) const { return { x OP other.x, y OP other.y, z OP other.z, w OP other.w }; } \
                        vec4 operator OP (T other) const { return { x OP other, y OP other, z OP other, w OP other }; } \
                        vec4& operator OP##= (const vec4& other) { x OP##= other.x; y OP##= other.y; z OP##= other.z; w OP##= other.w; return *this; } \
                        vec4& operator OP##= (T other) { x OP##= other; y OP##= other; z OP##= other; w OP##= other; return *this; }
        
        DEF_VEC4_OP(+)
        DEF_VEC4_OP(-)
        DEF_VEC4_OP(*)
        DEF_VEC4_OP(/)
        DEF_VEC4_OP(%)

        vec4 operator+() const { return *this; }
        vec4 operator-() const { return { -x, -y, -z, -w }; }
        
        bool operator==(const vec4& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
        bool operator!=(const vec4& other) const { return !(*this == other); }

#undef DEF_VEC3_OP
        template <class TOther> operator vec4<TOther>() { return { (TOther)x, (TOther)y, (TOther)z, (TOther)w }; }
        template <VecLessThan<vec4>    TVec> operator TVec() { return TVec { *this }; }
        template <VecGreaterThan<vec4> TVec> operator TVec() { return TVec { x, y, z, w }; }
    };
#pragma endregion

#pragma region Vec2 Definition
    template <class T> const vec2<T> vec2<T>::RIGHT = {  1,  0 };
    template <class T> const vec2<T> vec2<T>::LEFT  = { -1,  0 };
    template <class T> const vec2<T> vec2<T>::UP    = {  0,  1 };
    template <class T> const vec2<T> vec2<T>::DOWN  = {  0, -1 };
    
    template <class T> const vec2<T> vec2<T>::ZERO  = {  0,  0 };
    template <class T> const vec2<T> vec2<T>::ONE   = {  1,  1 };

    template <class T>
    vec2<T>::vec2(Direction2D dir, T scale) {
        using enum Direction2D;
        switch (dir) {
            case RIGHT: x =  scale; y =  0;     return;
            case LEFT : x = -scale; y =  0;     return;
            case UP   : x =  0;     y =  scale; return;
            case DOWN : x =  0;     y = -scale; return;
            
            case ZERO : x =  0;     y =  0;     return;
            case UNIT : x =  scale; y =  scale; return;
            default: x = 0; y = 0;
        }
    }

    template <class T>
    vec2<T>::vec2(Corner2D cor, T scale) {
        using enum Corner2D;
        // using namespace stdu;
        // if (cor < 0 || cor >= CENTER) {
        //     x = 0; y = 0;
        //     return;
        // }
        //
        // x = cor & SIDE_LEFT   ? -scale : scale;
        // y = cor & SIDE_BOTTOM ? -scale : scale;

        // default method
        switch (cor) {
            case TOP_RIGHT   : x =  scale; y =  scale; return;
            case TOP_LEFT    : x = -scale; y =  scale; return;
            case BOTTOM_RIGHT: x =  scale; y = -scale; return;
            case BOTTOM_LEFT : x = -scale; y = -scale; return;
            
            case CENTER: x = 0; y = 0;  return;
            default: x = 0; y = 0;
        }
    }

    template <class T> vec2<T>::vec2(const vec3<T>& xy) : x(xy.x), y(xy.y) {}
    template <class T> vec2<T>::vec2(const vec4<T>& xy) : x(xy.x), y(xy.y) {}
#pragma endregion

#pragma region Vec3 Definition
    template <class T> const vec3<T> vec3<T>::RIGHT = {  1,  0,  0 };
    template <class T> const vec3<T> vec3<T>::LEFT  = { -1,  0,  0 };
    template <class T> const vec3<T> vec3<T>::UP    = {  0,  1,  0 };
    template <class T> const vec3<T> vec3<T>::DOWN  = {  0, -1,  0 };
    template <class T> const vec3<T> vec3<T>::FRONT = {  0,  0,  1 };
    template <class T> const vec3<T> vec3<T>::BACK  = {  0,  0, -1 };
    
    template <class T> const vec3<T> vec3<T>::ZERO  = {  0,  0,  0 };
    template <class T> const vec3<T> vec3<T>::ONE   = {  1,  1,  1 };

    template <class T>
    vec3<T>::vec3(Direction3D dir, T scale) {
        using enum Direction3D;
        switch (dir) {
            case RIGHT: x =  scale; y =  0;     z =  0;     return;
            case LEFT : x = -scale; y =  0;     z =  0;     return;
            case UP   : x =  0;     y =  scale; z =  0;     return;
            case DOWN : x =  0;     y = -scale; z =  0;     return;
            case FRONT: x =  0;     y =  0;     z =  scale; return;
            case BACK : x =  0;     y =  0;     z = -scale; return;
            case ZERO : x =  0;     y =  0;     z =  0;     return;
            case UNIT : x =  scale; y =  scale; z =  scale; return;
            default: x = 0; y = 0; z = 0;
        }
    }

    template <class T>
    vec3<T>::vec3(Corner3D cor, T scale) {
        using enum Corner3D;
        switch (cor) {
            case FRONT_TOP_RIGHT   : x =  scale; y =  scale; z =  scale; return;
            case FRONT_TOP_LEFT    : x = -scale; y =  scale; z =  scale; return;
            case FRONT_BOTTOM_RIGHT: x =  scale; y = -scale; z =  scale; return;
            case FRONT_BOTTOM_LEFT : x = -scale; y = -scale; z =  scale; return;
            case BACK_TOP_RIGHT    : x =  scale; y =  scale; z = -scale; return;
            case BACK_TOP_LEFT     : x = -scale; y =  scale; z = -scale; return;
            case BACK_BOTTOM_RIGHT : x =  scale; y = -scale; z = -scale; return;
            case BACK_BOTTOM_LEFT  : x = -scale; y = -scale; z = -scale; return;
            case CENTER: x = 0; y = 0; z = 0; return;
            default: x = 0; y = 0; z = 0;
        }
    }
    
    template <class T> vec3<T>::vec3(const vec4<T>& xyz)         : x(xyz.x), y(xyz.y), z(xyz.z) {}
    template <class T> vec3<T>::vec3(const vec2<T>& xy, float z) : x(xy.x),  y(xy.y),  z(z) {}
#pragma endregion

#pragma region Vec4 Definition
    template <class T> const vec4<T> vec4<T>::RIGHT = {  1,  0,  0,  0 };
    template <class T> const vec4<T> vec4<T>::LEFT  = { -1,  0,  0,  0 };
    template <class T> const vec4<T> vec4<T>::UP    = {  0,  1,  0,  0 };
    template <class T> const vec4<T> vec4<T>::DOWN  = {  0, -1,  0,  0 };
    template <class T> const vec4<T> vec4<T>::FRONT = {  0,  0,  1,  0 };
    template <class T> const vec4<T> vec4<T>::BACK  = {  0,  0, -1,  0 };
    template <class T> const vec4<T> vec4<T>::IN    = {  0,  0,  0,  1 };
    template <class T> const vec4<T> vec4<T>::OUT   = {  0,  0,  0, -1 };
    
    template <class T> const vec4<T> vec4<T>::ZERO  = {  0,  0,  0,  0 };
    template <class T> const vec4<T> vec4<T>::ONE   = {  1,  1,  1,  1 };

    template <class T>
    vec4<T>::vec4(Direction4D dir, T scale) {
        using enum Direction4D;
        switch (dir) {
            case RIGHT: x =  scale; y =  0;     z =  0;     w =  0;     return;
            case LEFT : x = -scale; y =  0;     z =  0;     w =  0;     return;
            case UP   : x =  0;     y =  scale; z =  0;     w =  0;     return;
            case DOWN : x =  0;     y = -scale; z =  0;     w =  0;     return;
            case FRONT: x =  0;     y =  0;     z =  scale; w =  0;     return;
            case BACK : x =  0;     y =  0;     z = -scale; w =  0;     return;
            case IN   : x =  0;     y =  0;     z =  0;     w =  scale; return;
            case OUT  : x =  0;     y =  0;     z =  0;     w = -scale; return;
            case ZERO : x =  0;     y =  0;     z =  0;     w =  0;     return;
            case UNIT : x =  scale; y =  scale; z =  scale; w =  scale; return;
            default: x = 0; y = 0;
        }
    }

    template <class T>
    vec4<T>::vec4(Corner4D cor, T scale) {
        using enum Corner4D;
        switch (cor) {
            case INNER_FRONT_TOP_RIGHT   : x =  scale; y =  scale; z =  scale; w =  scale; return;
            case INNER_FRONT_TOP_LEFT    : x = -scale; y =  scale; z =  scale; w =  scale; return;
            case INNER_FRONT_BOTTOM_RIGHT: x =  scale; y = -scale; z =  scale; w =  scale; return;
            case INNER_FRONT_BOTTOM_LEFT : x = -scale; y = -scale; z =  scale; w =  scale; return;
            case INNER_BACK_TOP_RIGHT    : x =  scale; y =  scale; z = -scale; w =  scale; return;
            case INNER_BACK_TOP_LEFT     : x = -scale; y =  scale; z = -scale; w =  scale; return;
            case INNER_BACK_BOTTOM_RIGHT : x =  scale; y = -scale; z = -scale; w =  scale; return;
            case INNER_BACK_BOTTOM_LEFT  : x = -scale; y = -scale; z = -scale; w =  scale; return;
            case OUTER_FRONT_TOP_RIGHT   : x =  scale; y =  scale; z =  scale; w = -scale; return;
            case OUTER_FRONT_TOP_LEFT    : x = -scale; y =  scale; z =  scale; w = -scale; return;
            case OUTER_FRONT_BOTTOM_RIGHT: x =  scale; y = -scale; z =  scale; w = -scale; return;
            case OUTER_FRONT_BOTTOM_LEFT : x = -scale; y = -scale; z =  scale; w = -scale; return;
            case OUTER_BACK_TOP_RIGHT    : x =  scale; y =  scale; z = -scale; w = -scale; return;
            case OUTER_BACK_TOP_LEFT     : x = -scale; y =  scale; z = -scale; w = -scale; return;
            case OUTER_BACK_BOTTOM_RIGHT : x =  scale; y = -scale; z = -scale; w = -scale; return;
            case OUTER_BACK_BOTTOM_LEFT  : x = -scale; y = -scale; z = -scale; w = -scale; return;
            case CENTER: x = 0; y = 0; z = 0; return;
            default: x = 0; y = 0; z = 0;
        }
    }

    template <class T> vec4<T>::vec4(const vec2<T>& xy, float z, float w)  : x(xy.x),  y(xy.y),  z(z),     w(w) {}
    template <class T> vec4<T>::vec4(const vec2<T>& xy, const vec2<T>& zw) : x(xy.x),  y(xy.y),  z(zw.z),  w(zw.w) {}
    template <class T> vec4<T>::vec4(const vec3<T>& xyz, float w)          : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
#pragma endregion
}
