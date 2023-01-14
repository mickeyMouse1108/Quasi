#include "Vector.h"

namespace Maths
{
#pragma region Vector2
    Vector2 const Vector2::RIGHT = {  1.0f,  0.0f };
    Vector2 const Vector2::LEFT  = { -1.0f,  0.0f };
    Vector2 const Vector2::UP    = {  0.0f,  1.0f };
    Vector2 const Vector2::DOWN  = {  0.0f, -1.0f };
    
    Vector2::Vector2(const Vector3& vec3) : x(vec3.x), y(vec3.y) {}

    Vector2 operator+(const Vector2& a, const Vector2& b) { return {a.x + b.x, a.y + b.y}; }
    Vector2 operator-(const Vector2& a, const Vector2& b) { return {a.x - b.x, a.y - b.y}; }
    Vector2 operator*(const Vector2& a, const Vector2& b) { return {a.x * b.x, a.y * b.y}; }
    Vector2 operator/(const Vector2& a, const Vector2& b) { return {a.x / b.x, a.y / b.y}; }
    Vector2 operator+(const Vector2& a, float b)          { return {a.x + b, a.y + b}; }
    Vector2 operator-(const Vector2& a, float b)          { return {a.x - b, a.y - b}; }
    Vector2 operator*(const Vector2& a, float b)          { return {a.x * b, a.y * b}; }
    Vector2 operator/(const Vector2& a, float b)          { return {a.x / b, a.y / b}; }
#pragma endregion

#pragma region Vector3
    Vector3 const Vector3::RIGHT = {  1.0f,  0.0f,  0.0f };
    Vector3 const Vector3::LEFT  = { -1.0f,  0.0f,  0.0f };
    Vector3 const Vector3::UP    = {  0.0f,  1.0f,  0.0f };
    Vector3 const Vector3::DOWN  = {  0.0f, -1.0f,  0.0f };
    Vector3 const Vector3::FRONT = {  0.0f,  0.0f,  1.0f };
    Vector3 const Vector3::BACK  = {  0.0f,  0.0f, -1.0f };
    
    Vector3::Vector3(const Vector4& vec4) : x(vec4.x), y(vec4.y), z(vec4.z) {}

    Vector3 operator+(const Vector3& a, const Vector3& b) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
    Vector3 operator-(const Vector3& a, const Vector3& b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
    Vector3 operator*(const Vector3& a, const Vector3& b) { return {a.x * b.x, a.y * b.y, a.z * b.z}; }
    Vector3 operator/(const Vector3& a, const Vector3& b) { return {a.x / b.x, a.y / b.y, a.z / b.z}; }
    Vector3 operator+(const Vector3& a, float b)          { return {a.x + b, a.y + b, a.z + b}; }
    Vector3 operator-(const Vector3& a, float b)          { return {a.x - b, a.y - b, a.z - b}; }
    Vector3 operator*(const Vector3& a, float b)          { return {a.x * b, a.y * b, a.z * b}; }
    Vector3 operator/(const Vector3& a, float b)          { return {a.x / b, a.y / b, a.z / b}; }
#pragma endregion

#pragma region Vector4
    Vector4 const Vector4::RIGHT = {  1.0f,  0.0f,  0.0f,  0.0f };
    Vector4 const Vector4::LEFT  = { -1.0f,  0.0f,  0.0f,  0.0f };
    Vector4 const Vector4::UP    = {  0.0f,  1.0f,  0.0f,  0.0f };
    Vector4 const Vector4::DOWN  = {  0.0f, -1.0f,  0.0f,  0.0f };
    Vector4 const Vector4::FRONT = {  0.0f,  0.0f,  1.0f,  0.0f };
    Vector4 const Vector4::BACK  = {  0.0f,  0.0f, -1.0f,  0.0f };
    Vector4 const Vector4::IN    = {  0.0f,  0.0f,  0.0f,  1.0f };
    Vector4 const Vector4::OUT   = {  0.0f,  0.0f,  0.0f, -1.0f };
    
    Vector4 operator+(const Vector4& a, const Vector4& b) { return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w}; }
    Vector4 operator-(const Vector4& a, const Vector4& b) { return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w}; }
    Vector4 operator*(const Vector4& a, const Vector4& b) { return {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w}; }
    Vector4 operator/(const Vector4& a, const Vector4& b) { return {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w}; }
    Vector4 operator+(const Vector4& a, float b)          { return {a.x + b, a.y + b, a.z + b, a.w + b}; }
    Vector4 operator-(const Vector4& a, float b)          { return {a.x - b, a.y - b, a.z - b, a.w + b}; }
    Vector4 operator*(const Vector4& a, float b)          { return {a.x * b, a.y * b, a.z * b, a.w + b}; }
    Vector4 operator/(const Vector4& a, float b)          { return {a.x / b, a.y / b, a.z / b, a.w + b}; }
#pragma endregion
}