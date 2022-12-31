#pragma once

namespace Maths
{
    class Vector4;
    class Vector3;

    class Vector2
    {
    public:
        float x, y;
        Vector2() : x(0), y(0) {}
        Vector2(float x, float y = 0) : x(x), y(y) {}
        Vector2(float val) : x(val), y(val) {}
        Vector2(const Vector3& vec3);

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

    class Vector3
    {
    public:
        float x, y, z;
        Vector3() : x(0), y(0), z(0) {}
        Vector3(float x, float y, float z = 0) : x(x), y(y), z(z) {}
        Vector3(float val) : x(val), y(val), z(val) {}
        Vector3(const Vector2& vec2, float z) : x(vec2.x), y(vec2.y), z(z) {}
        Vector3(const Vector4& vec4);

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

    class Vector4
    {
    public:
        float x, y, z, w;
        Vector4() : x(0), y(0), z(0), w(0) {}
        Vector4(float x, float y, float z, float w = 0) : x(x), y(y), z(z), w(w) {}
        Vector4(float val) : x(val), y(val), z(val), w(val) {}
        Vector4(const Vector3& vec3, float w = 0) : x(vec3.x), y(vec3.y), z(vec3.z), w(w) {}

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
}