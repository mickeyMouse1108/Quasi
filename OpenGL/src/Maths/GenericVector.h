#pragma once
#include "Vector.h"

namespace Maths
{
    template <class T> struct Vec2;
    template <class T> struct Vec3;
    template <class T> struct Vec4;

#pragma region Vec2 Declaration
    template <class T> struct Vec2
    {
        T x, y;
        Vec2() : x(0), y(0) {}
        Vec2(T x, T y = 0) : x(x), y(y) {}
        Vec2(T val) : x(val), y(val) {}
        Vec2(const Vec3<T>& vec3);

        template <class TOther> Vec2<TOther> Cast () { return {(TOther)x, (TOther)y}; }
                                Vector2      CastF() { return {(float) x, (float) y}; }

        T operator[] (unsigned int i) const { return ((const T*)this)[i]; }
        operator T*() { return (T*)this; }
    };
    
    template <class T> Vec2<T> operator+ (const Vec2<T>&a, const Vec2<T>& b);
    template <class T> Vec2<T> operator- (const Vec2<T>&a, const Vec2<T>& b);
    template <class T> Vec2<T> operator* (const Vec2<T>&a, const Vec2<T>& b);
    template <class T> Vec2<T> operator/ (const Vec2<T>&a, const Vec2<T>& b);
    template <class T> Vec2<T> operator+ (const Vec2<T>&a, T b);
    template <class T> Vec2<T> operator- (const Vec2<T>&a, T b);
    template <class T> Vec2<T> operator* (const Vec2<T>&a, T b);
    template <class T> Vec2<T> operator/ (const Vec2<T>&a, T b);
#pragma endregion 

#pragma region Vec3 Declaration
    template <class T> struct Vec3
    {
        T x, y, z;
        Vec3() : x(0), y(0), z(0) {}
        Vec3(T x, T y, T z = 0) : x(x), y(y), z(z) {}
        Vec3(T val) : x(val), y(val), z(val) {}
        Vec3(const Vec2<T>& vec2, T z) : x(vec2.x), y(vec2.y), z(z) {}
        Vec3(const Vec4<T>& vec4);

        template <class TOther> Vec3<TOther> Cast () { return {(TOther)x, (TOther)y, (TOther)z}; }
                                Vector3      CastF() { return {(float) x, (float) y, (float) z}; }

        T operator[] (unsigned int i) const { return ((const T*)this)[i]; }
        operator T*() { return (T*)this; }
    };

    template <class T> Vec3<T> operator+ (const Vec3<T>&a, const Vec3<T>& b);
    template <class T> Vec3<T> operator- (const Vec3<T>&a, const Vec3<T>& b);
    template <class T> Vec3<T> operator* (const Vec3<T>&a, const Vec3<T>& b);
    template <class T> Vec3<T> operator/ (const Vec3<T>&a, const Vec3<T>& b);
    template <class T> Vec3<T> operator+ (const Vec3<T>&a, T b);
    template <class T> Vec3<T> operator- (const Vec3<T>&a, T b);
    template <class T> Vec3<T> operator* (const Vec3<T>&a, T b);
    template <class T> Vec3<T> operator/ (const Vec3<T>&a, T b);
#pragma endregion

#pragma region Vec4 Declaration
    template <class T> struct Vec4
    {
        T x, y, z, w;
        Vec4() : x(0), y(0), z(0), w(0) {}
        Vec4(T x, T y, T z, T w = 0) : x(x), y(y), z(z), w(w) {}
        Vec4(T val) : x(val), y(val), z(val), w(val) {}
        Vec4(const Vec3<T>& vec3, T w = 0) : x(vec3.x), y(vec3.y), z(vec3.z), w(w) {}

        template <class TOther> Vec2<TOther> Cast () { return {(TOther)x, (TOther)y, (TOther)z, (TOther)w}; }
                                Vector2      CastF() { return {(float) x, (float) y, (float) z, (float) w}; }

        T operator[] (unsigned int i) const { return ((const T*)this)[i]; }
        operator T*() { return (T*)this; }
    };

    template <class T> Vec4<T> operator+ (const Vec4<T>&a, const Vec4<T>& b);
    template <class T> Vec4<T> operator- (const Vec4<T>&a, const Vec4<T>& b);
    template <class T> Vec4<T> operator* (const Vec4<T>&a, const Vec4<T>& b);
    template <class T> Vec4<T> operator/ (const Vec4<T>&a, const Vec4<T>& b);
    template <class T> Vec4<T> operator+ (const Vec4<T>&a, T b);
    template <class T> Vec4<T> operator- (const Vec4<T>&a, T b);
    template <class T> Vec4<T> operator* (const Vec4<T>&a, T b);
    template <class T> Vec4<T> operator/ (const Vec4<T>&a, T b);
#pragma endregion

#pragma region Vec2 Definition
    template <class T> Vec2<T>::Vec2(const Vec3<T>& vec3) : x(vec3.x), y(vec3.y) {}

    template <class T> Vec2<T> operator+(const Vec2<T>& a, const Vec2<T>& b) { return {a.x + b.x, a.y + b.y}; }
    template <class T> Vec2<T> operator-(const Vec2<T>& a, const Vec2<T>& b) { return {a.x - b.x, a.y - b.y}; }
    template <class T> Vec2<T> operator*(const Vec2<T>& a, const Vec2<T>& b) { return {a.x * b.x, a.y * b.y}; }
    template <class T> Vec2<T> operator/(const Vec2<T>& a, const Vec2<T>& b) { return {a.x / b.x, a.y / b.y}; }
    template <class T> Vec2<T> operator+(const Vec2<T>& a, T b)              { return {a.x + b, a.y + b}; }
    template <class T> Vec2<T> operator-(const Vec2<T>& a, T b)              { return {a.x - b, a.y - b}; }
    template <class T> Vec2<T> operator*(const Vec2<T>& a, T b)              { return {a.x * b, a.y * b}; }
    template <class T> Vec2<T> operator/(const Vec2<T>& a, T b)              { return {a.x / b, a.y / b}; }
#pragma endregion

#pragma region Vec3 Definition
    template <class T> Vec3<T>::Vec3(const Vec4<T>& vec4) : x(vec4.x), y(vec4.y), z(vec4.z) {}

    template <class T> Vec3<T> operator+(const Vec3<T>& a, const Vec3<T>& b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
    template <class T> Vec3<T> operator-(const Vec3<T>& a, const Vec3<T>& b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
    template <class T> Vec3<T> operator*(const Vec3<T>& a, const Vec3<T>& b) { return {a.x * b.x, a.y * b.y, a.z * b.z}; }
    template <class T> Vec3<T> operator/(const Vec3<T>& a, const Vec3<T>& b) { return {a.x / b.x, a.y / b.y, a.z / b.z}; }
    template <class T> Vec3<T> operator+(const Vec3<T>& a, T b)              { return {a.x + b, a.y + b, a.z + b}; }
    template <class T> Vec3<T> operator-(const Vec3<T>& a, T b)              { return {a.x - b, a.y - b, a.z - b}; }
    template <class T> Vec3<T> operator*(const Vec3<T>& a, T b)              { return {a.x * b, a.y * b, a.z * b}; }
    template <class T> Vec3<T> operator/(const Vec3<T>& a, T b)              { return {a.x / b, a.y / b, a.z / b}; }
#pragma endregion

#pragma region Vec4 Definition
    template <class T> Vec4<T> operator+(const Vec4<T>& a, const Vec4<T>& b) { return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w}; }
    template <class T> Vec4<T> operator-(const Vec4<T>& a, const Vec4<T>& b) { return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w}; }
    template <class T> Vec4<T> operator*(const Vec4<T>& a, const Vec4<T>& b) { return {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w}; }
    template <class T> Vec4<T> operator/(const Vec4<T>& a, const Vec4<T>& b) { return {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w}; }
    template <class T> Vec4<T> operator+(const Vec4<T>& a, T b)              { return {a.x + b, a.y + b, a.z + b, a.w + b}; }
    template <class T> Vec4<T> operator-(const Vec4<T>& a, T b)              { return {a.x - b, a.y - b, a.z - b, a.w - b}; }
    template <class T> Vec4<T> operator*(const Vec4<T>& a, T b)              { return {a.x * b, a.y * b, a.z * b, a.w * b}; }
    template <class T> Vec4<T> operator/(const Vec4<T>& a, T b)              { return {a.x / b, a.y / b, a.z / b, a.w / b}; }
#pragma endregion

    using Vec2Int = Vec2<int>;
    using Vec3Int = Vec3<int>;
    using Vec4Int = Vec4<int>;
}