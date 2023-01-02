#pragma once

namespace Maths
{
    struct Vector2;
    struct Vector3;
    struct Vector4;

    template <class T> struct Vec2;
    template <class T> struct Vec3;
    template <class T> struct Vec4;

#pragma region Vector2 Declaration
    struct Vector2
    {
        float x, y;
        Vector2() : x(0), y(0) {}
        Vector2(float x, float y = 0) : x(x), y(y) {}
        Vector2(float val) : x(val), y(val) {}
        Vector2(const Vector3& vec3);

        template <class TOther> Vec2<TOther> Cast() { return {(TOther)x, (TOther)y}; }

        float operator[] (unsigned int i) const { return ((const float*)this)[i]; }
        operator float*() { return (float*)this; }
    };

    Vector2 operator+ (const Vector2&a, const Vector2& b);
    Vector2 operator- (const Vector2&a, const Vector2& b);
    Vector2 operator* (const Vector2&a, const Vector2& b);
    Vector2 operator/ (const Vector2&a, const Vector2& b);
    Vector2 operator+ (const Vector2&a, float b);
    Vector2 operator- (const Vector2&a, float b);
    Vector2 operator* (const Vector2&a, float b);
    Vector2 operator/ (const Vector2&a, float b);
#pragma endregion 

#pragma region Vector3 Declaration
    struct Vector3
    {
        float x, y, z;
        Vector3() : x(0), y(0), z(0) {}
        Vector3(float x, float y, float z = 0) : x(x), y(y), z(z) {}
        Vector3(float val) : x(val), y(val), z(val) {}
        Vector3(const Vector2& vec2, float z) : x(vec2.x), y(vec2.y), z(z) {}
        Vector3(const Vector4& vec4);

        template <class TOther> Vec3<TOther> Cast () { return {(TOther)x, (TOther)y, (TOther)z}; }

        float operator[] (unsigned int i) const { return ((const float*)this)[i]; }
        operator float*() { return (float*)this; }
    };

    Vector3 operator+ (const Vector3&a, const Vector3& b);
    Vector3 operator- (const Vector3&a, const Vector3& b);
    Vector3 operator* (const Vector3&a, const Vector3& b);
    Vector3 operator/ (const Vector3&a, const Vector3& b);
    Vector3 operator+ (const Vector3&a, float b);
    Vector3 operator- (const Vector3&a, float b);
    Vector3 operator* (const Vector3&a, float b);
    Vector3 operator/ (const Vector3&a, float b);
#pragma endregion 

#pragma region Vector4 Declaration
    struct Vector4
    {
        float x, y, z, w;
        Vector4() : x(0), y(0), z(0), w(0) {}
        Vector4(float x, float y, float z, float w = 0) : x(x), y(y), z(z), w(w) {}
        Vector4(float val) : x(val), y(val), z(val), w(val) {}
        Vector4(const Vector3& vec3, float w = 0) : x(vec3.x), y(vec3.y), z(vec3.z), w(w) {}

        template <class TOther> Vec2<TOther> Cast () { return {(TOther)x, (TOther)y, (TOther)z, (TOther)w}; }

        float operator[] (unsigned int i) const { return ((const float*)this)[i]; }
        operator float*() { return (float*)this; }
    };

    Vector4 operator+ (const Vector4&a, const Vector4& b);
    Vector4 operator- (const Vector4&a, const Vector4& b);
    Vector4 operator* (const Vector4&a, const Vector4& b);
    Vector4 operator/ (const Vector4&a, const Vector4& b);
    Vector4 operator+ (const Vector4&a, float b);
    Vector4 operator- (const Vector4&a, float b);
    Vector4 operator* (const Vector4&a, float b);
    Vector4 operator/ (const Vector4&a, float b);
#pragma endregion 
}