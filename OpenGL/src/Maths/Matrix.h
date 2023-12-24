#pragma once
#include <ostream>
#include "opengl.h"
#include "Vector.h"

namespace Maths
{
    class Matrix3D
    {
    private:
        float m11, m12, m13, m14;
        float m21, m22, m23, m24;
        float m31, m32, m33, m34;
        float m41, m42, m43, m44;
    public:
        Matrix3D() : m11(1.0f), m12(0.0f), m13(0.0f), m14(0.0f),
                     m21(0.0f), m22(1.0f), m23(0.0f), m24(0.0f),
                     m31(0.0f), m32(0.0f), m33(1.0f), m34(0.0f),
                     m41(0.0f), m42(0.0f), m43(0.0f), m44(1.0f) {}
        OPENGL_API Matrix3D(float vals[16], bool horizontalPacking = true);
        OPENGL_API Matrix3D(float m11, float m12, float m13, float m14,
                 float m21, float m22, float m23, float m24,
                 float m31, float m32, float m33, float m34,
                 float m41, float m42, float m43, float m44,
                 bool horizontalPacking = true);
        OPENGL_API Matrix3D(Vector4 arrs[4], bool verticalPacking = true);
        OPENGL_API Matrix3D(const Vector4& r1, const Vector4& r2, const Vector4& r3, const Vector4& r4, bool verticalPacking = true);

        OPENGL_API const float* GetInRow() const;
        OPENGL_API const Vector4* GetRows() const;

        OPENGL_API void Translate(const Vector3& translation);
        OPENGL_API static Matrix3D TranslateMat(const Vector3& translation, const Matrix3D& mat = {});
        OPENGL_API void Scale(const Vector3& scale);
        OPENGL_API static Matrix3D ScaleMat(const Vector3& scale, const Matrix3D& mat = {});
        OPENGL_API void Rotate(const Vector3& rotation);
        OPENGL_API static Matrix3D RotateMat(const Vector3& rotation, const Matrix3D& mat = {});

        OPENGL_API static Matrix3D OrthoProjection(float left, float right, float down, float up, float near, float front);
        OPENGL_API static Matrix3D PerspectiveProjection(float left, float right, float down, float up, float near, float far);
        OPENGL_API static Matrix3D PerspectiveProjectionFOV(float fovDeg, float aspect, float near, float far);
        OPENGL_API static Matrix3D Transform(const Vector3& translate, const Vector3& scale, const Vector3& rotate);

        OPENGL_API [[nodiscard]] Matrix3D Transpose() const;
        OPENGL_API [[nodiscard]] float Determinate() const;
        OPENGL_API [[nodiscard]] Matrix3D Adjugate() const;
        OPENGL_API [[nodiscard]] Matrix3D Inverse() const;

        OPENGL_API static Matrix3D UnitAxis(Direction3D x, Direction3D y, Direction3D z);

        OPENGL_API float operator[](unsigned i) { return *((float*)this + i); }

        OPENGL_API Vector3 operator* (const Vector3& v) const;
        OPENGL_API Vector4 operator* (const Vector4& v) const;
        OPENGL_API Matrix3D operator* (const Matrix3D& m) const;
        OPENGL_API Matrix3D operator* (float x) const;
        
        friend OPENGL_API std::ostream& operator<<(std::ostream& stream, const Matrix3D& mat);
    };

    OPENGL_API std::ostream& operator<<(std::ostream& stream, const Matrix3D& mat);
}